#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/pci.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <asm/io.h>
 
int data;
int got_irq = 0;
static irqreturn_t gpio_irq_test(int irq, void *dev_id)
{
    pr_err("got one irq\n");
    got_irq = 1;
    return IRQ_HANDLED;
}

static int __init test_init(void)
{
    int irq;
    int retval;
    struct device_node *np;
    got_irq = 0;
    np = of_find_node_by_name(NULL, "test");
    if (NULL != np) {
        irq = of_irq_get(np, 0);
        pr_err("request irq %d\n", irq);
        retval = request_irq(irq, gpio_irq_test,
                                     IRQF_ONESHOT | IRQF_TRIGGER_LOW , "gpiotest", &data);
        pr_err("retval %d\n", retval);
        if(retval) 
            return -1;
        while(got_irq == 0);
            free_irq(irq, &data);
    }
    pr_err("exit\n");
}

static void __exit test_exit(void)
{

}

module_init(test_init);
module_exit(test_exit);
 
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("test driver");
 
&test {
        compatible="sdrvtest";
        interrupt-parent=<&port4a>;
        interrupts = <2 IRQ_TYPE_LEVEL_LOW>;
        //interrupt-names="test";
        status="okay";
};
 