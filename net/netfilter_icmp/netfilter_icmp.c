// Author : Ramin Farajpour Cami

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/inet.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/icmp.h>
#include <linux/proc_fs.h>
#include <linux/netlink.h>
#include <linux/mroute.h>
#include <net/checksum.h>
#include <net/inet_ecn.h>
#include <net/xfrm.h>
#include <net/route.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/arp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <net/inetpeer.h>
#include <net/protocol.h>
#include <net/flow.h>
#include <asm/types.h>



struct proc_dir_entry *netfilter_icmp_proc_a;

int drop_pkts=0;

#define PROCFS_MAX_SIZE 30
char proc_buf[PROCFS_MAX_SIZE];

static ssize_t netfilter_icmp_proc_a_write(struct file *fp, const char *buf, size_t len, loff_t * off)
{
	int val=0;
	if(len > PROCFS_MAX_SIZE) {
     		return -EFAULT;
   	}
   	if(copy_from_user(proc_buf, buf, len)) {
     		return -EFAULT;
   	}

	val=simple_strtoul(proc_buf,NULL,10);
   	if(val==0 || val==1) {
      		drop_pkts=val;
     	}
	return len;
}

static ssize_t netfilter_icmp_proc_a_read(struct file *fp, char *buf, size_t len, loff_t * off)
{
  	static int finished=0;
  	if(finished) {
    		finished=0;
    		return 0;
  	}
  	finished=1;

	if(drop_pkts) {
     		sprintf(buf, "drop_pkts: true\n");
    	}
        else {
         	 sprintf(buf, "drop_pkts: false\n");
        }
	return strlen(buf);

}

static struct file_operations netfilter_icmp_proc_a_fops = { .owner=THIS_MODULE, .read=netfilter_icmp_proc_a_read, .write=netfilter_icmp_proc_a_write, };

static struct nf_hook_ops nfho_pre_routing;

unsigned int pre_routing_hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	if(drop_pkts) {
    		return NF_DROP;
  	}
	return NF_ACCEPT;
}
void netfilter_icmp_init_proc(void){

  netfilter_icmp_proc_a = proc_create( "netfilter_proc_a", 0666, NULL, &netfilter_icmp_proc_a_fops);
 	if(netfilter_icmp_proc_a==NULL) {
  	  	printk(KERN_ALERT "Error: Could not initialize %s\n", "netfilter_proc_a");
        	goto init_end;
  	}

	nfho_pre_routing.hook = pre_routing_hook_func;
	nfho_pre_routing.hooknum = NF_INET_PRE_ROUTING;
	nfho_pre_routing.pf = PF_INET;
	nfho_pre_routing.priority = NF_IP_PRI_FIRST;
	nf_register_hook(&nfho_pre_routing);

init_end:
	return 0;

}
