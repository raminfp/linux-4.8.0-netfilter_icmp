#ifndef _NETFILTER_NETFILTER_ICMP_H
#define _NETFILTER_NETFILTER_ICMP_H
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <linux/ctype.h>

#define proc_dir_name   		"netfilter"
#define proc_file_tlc_proc_a_name	"netfilter_icmp"

void netfilter_icmp_init_proc(void);
#endif

