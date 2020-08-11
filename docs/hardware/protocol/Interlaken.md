## 1. 应用

Interlaken有多种应用，可以在不同媒介中传输：

- 帧/以太网MAC到NPU或L2/L3交换芯片

![](../10281212/images/interlaken应用1.png)

- 线卡上交换侧接口和交换板之间

![](../10281212/images/interlaken应用2.png)

## 2. 特性

1. 支持256个通道，最高可拓展到64K
2. 通过配置控制字格式来表示数据包结构
3. 通过配置Meta frame结构完成lane之间的对齐
4. 协议本身与serdes速率和lane数量无关
5. 提供带外和带内流控方式，通过Xon/Xoff来控制流量
6. 采用64/67B编码格式1. 接口性能跟lane数量成正比

## 3. 协议

### 3.1 协议介绍

Interlaken协议包含了两种基本结构：数据传输格式和Meta frame。

数据传输格式是将数据分割成若干个busrt，每个burst是原始数据的子集，每个burst由两个控制字限定，前后各一个，并且控制字内的字段设定影响控制字前后的数据。报文通过一个或多个burst被顺序的发送出去，同时burst的大小可配置（通过burstmax和burstshort配置）。这种将报文数据分割成burst传送的方式可以提升性能，降低延迟。

Meta frame是一种支持serdes传输而定义的，利用数据传输的带内运行，使用控制字的特定格式与数据传输方式区分开。

### 3.2 数据格式

Interlaken通过接口发送的数据基本单位是8字节，符合64/67B编码，数据传输通过两种数据类型实现，数据字和控制字，这两种字是通过bit64 ~ bit65来区分的，两种格式如下：

![](../10281212/images/interlaken格式.svg)

burst/idle控制字结构

|Bit|Field|功能|
|:----|:----|:----|
|66|inversion|反转位，1表示0 ~ 63所有位取反，0表示0 ~ 63所有位保持|
|65:64|farming|区分控制字和数据字，01表示数据字，10表示控制字|
|63|Control|1表示burst/idle控制字，0表示Meta frame控制字|
|62|Type|设置为1时，通道号和SOP字段有效，并且在控制字后有一个数据Burst；如果设置为0，通道号和SOP字段无效，并且控制字后无数据|
|61|SOP|如果设置为1，控制字之后的Burst表示数据包的开始，如果设置为0，表示Burst是数据包的中间或结尾|
|60:57|EOP_format|包结束指示符|
|56|reset calendar|设置为1表示带内流控状态复位|
|55:40|in-band flow control|当前16个channel的1位流控制状态;如果设置为“1”，则channel是XON，如果设置为“0”，则channel是XOFF|
|39:32|channel number|与控制字之后的Burst相关联的信道; 空闲控制字时全为0|
|31:24|multiple-use|可做流控控制位或burst通道复用|
|23:0|CRC24|CRC错误检查|

### 3.3 burst传输

在协议介绍中提到burst的大小可以配置，主要通过如下两个参数：
- BurstMax：数据块的最大长度（64B的倍数）
- BurstShort：两个burst控制字之间最小间隔（最小32B，增量为8B）
- BurstMin：与EOP相关的最小长度，下面介绍的Optional Scheduling Enhancement就与BurstMin有关

接口通常在控制字之后发送BurstMax长度的Burst。发送设备中的调度逻辑可以根据流控制状态信息来自由选择服务信道的顺序，Burst在每个信道上依次发送，直到报文被发送完，然后可以开始新的报文传输，以lane0 ~ laneM为例，简单的传输示意图如下：

![](../10281212/images/interlaken_lane传输.svg)

当报文长度/BurstMax的值很小时，使得在最后一个BurstMax之后还剩余少量数据要传输，此时burstShort参数保证了连续的Burst控制字之间的间隔。最小BurstShort间隔为32B，以8B为增量，通过在最后一个Burst控制字之后添加额外的idle控制字来强制执行BurstShort，在最坏的情况下，浪费的带宽等于（BurstShort-1）字节，如下所示：

![](../10281212/images/interlakne_burstshot.svg)

这种情况，为了确保实现更为高效的调度，引入了上文提到的BurstMin参数，它定义为32B的倍数，必须服从于如下两点规则：
- burstmin <= burstmax/2
- burstmin >= burstshort

使用如下算法示例来说明此burstmin参数的用法及目的，同时定义如下参数：
- Packet_length=数据包的总长度
- Packet_remainder=一旦数据传输开始，仍要发送的数据包中的数据量
- Data_transfer=当前Burst上传输的数据量
- i=传输数据包所需的Burst数

算法代码如下：
```c
packet_remainder = packet_length
for (x=1; x <= i; ++x)
{
    if (packet_remainder >= BurstMax + BurstMin)
        data_transfer = BurstMax;
    else
    {
        if ((packet_length % BurstMax < BurstMin) && (packet_remainder > BurstMax))
            data_transfer = BurstMax - BurstMin;
        else
            data_transfer = packet_remainder;
    }
    packet_remainder = packet_remainder - data_transfer;
}
```

功能可以保证报文的最后一个Burst是BurstMini和BurstMax之间的尺寸，从而避免了多个短结束报文分段造成的背靠背的问题。

当要传送的报文长度为513字节时，设定burstmax=256字节，burstmin=64字节，513字节的报文总计要发送三个burst：
- burst1=burstmax=256字节
- burst2=burstmax-burstmin=256-64=192字节
- burst3=packet_remainder=65字节

如果报文长度只有511字节，那么只要发送两个burst:
- burst1=burstmax=256字节
- burst2= packet_remainder =255字节

具体实现可以根据需求调整brustmax和burstmin参数，服从上面的约束。

## 4. 流控

需求：在流量传输过程中，为防止缓冲器溢出，实现板载设备之间速率匹配，必须进行流量控制。

![](../10281212/images/流控示意图.svg)

原理：Interlaken提供简单的开关指示，俗称为Xon/Xoff（协议中规定1/0，但部分芯片会取反），指示传输端何时停止和开始发送数据包。Interlaken终端设备一般带有单通道缓冲器，并具有可编程的流量控制阈值，当缓冲器被填充至高于其阈值上限时，终端设备将Xoff发送至Interlaken源设备，通知源设备停止向该通道发送数据；相反的，一旦缓冲器排空低于阈值下限时，终端设备向Interlaken源设备发送Xon，通知源设备再次向该通道发送数据包。

在设置缓冲器大小和阈值时，必须考虑通道速率、流量控制延时、源调度响应和其他因素。如果阈值与缓冲器正确设置，在终端设备的数据包丢失将大大减少，线路也会得到充分利用。

Interlaken协议中有两种方法发送Xon/Xoff流量控制信息，分为带内流量控制和带外流量控制。

### 4.1 带内流量控制

带内流量控制是在burst控制字中执行的，一般用于源设备与终端设备位于相同设备时的双向应用。

带内流控在burst控制字段内配置，bit40 ~ bit55是16个通道，当16个通道不够用时，也可将bit24 ~ bit31拓展为额外的通道，共计有24个通道，通过每个通道上的状态位1或0来控制通道数据的发送和停止。

### 4.2 带外流量控制

带外流量控制是一般用于单向应用，使用源同步接口来实现，有如下三种信号：

FC_CLK：流控数据时钟信号

FC_DATA：流控状态信息

FC_SYNC：用于识别流控复位启动的同步信号

一般通过示波器，可以清晰的观察到CLK/DATA/SYNC三组信号的变化曲线，且interlaken是双边沿采样，相位正常且传输结果没有CRC错误就配置正常了。