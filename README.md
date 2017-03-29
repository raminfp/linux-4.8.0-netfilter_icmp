# Anatomy of a linux kernel development

Questions :
1) How to work kernel code?
2) How to write C code on the kernel?
3) How to building and install the kernel on old version linux?


I going to write about all question of the linux kernel and you learning write your own code on linux kernel and build it,

1- Downloading repo `linux-4.8.0-netfilter_icmp` :

```
root@raminfp ~# cd /tmp/
root@raminfp /tmp# 
root@raminfp /tmp# git clone https://github.com/raminfp/linux-4.8.0-netfilter_icmp
```

2- Source C code [netfilter_icmp](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/tree/master/net/netfilter_icmp) :

```
root@raminfp /h/r/D/l/linux-4.8.0# ls
arch/   COPYING  debian.master/  firmware/  ipc/     lib/         net/            samples/        sound/   usr/
block/  CREDITS  Documentation/  fs/        Kbuild   MAINTAINERS  README          scripts/        spl/     virt/
build/  crypto/  drivers/        include/   Kconfig  Makefile     README.md       security/       tools/   zfs/
certs/  debian/  dropped.txt     init/      kernel/  mm/          REPORTING-BUGS  snapcraft.yaml  ubuntu/
root@raminfp /tmp/linux-4.8.0# cd net/netfilter_icmp/
root@raminfp /tmp/linux-4.8.0/netfilter_icmp# 
root@raminfp /tmp/linux-4.8.0/netfilter_icmp# ls
Kconfig  Makefile  netfilter_icmp.c
root@raminfp /tmp/linux-4.8.0/netfilter_icmp# 
```

we see `netfilter_icmp.c` [here](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/blob/master/net/netfilter_icmp/netfilter_icmp.c) when you buil and install this kernel you are unabel to connect to `ping google.com`, with this code able to drop icmp packet on RING 0,

we see `Kconfig` file for configuration `menuconfig`,i see on `make menuconfig` config on image,and define `Makefile` for compile object,

back to `net` directory, 
```
root@raminfp /tmp/linux-4.8.0/net# pwd
/tmp/linux-4.8.0/net
root@raminfp /tmp/linux-4.8.0/net# ls
6lowpan/    ax25/        ceph/     dns_resolver/  ipv6/    key/       mac802154/       netlabel/     phonet/  sched/        tipc/       xfrm/
802/        batman-adv/  compat.c  dsa/           ipx/     l2tp/      Makefile         netlink/      qrtr/    sctp/         unix/
8021q/      bluetooth/   core/     ethernet/      irda/    l3mdev/    mpls/            netrom/       rds/     socket.c      vmw_vsock/
9p/         bridge/      dcb/      hsr/           iucv/    lapb/      ncsi/            nfc/          rfkill/  sunrpc/       wimax/
appletalk/  caif/        dccp/     ieee802154/    kcm/     llc/       netfilter/       openvswitch/  rose/    switchdev/    wireless/
atm/        can/         decnet/   ipv4/          Kconfig  mac80211/  netfilter_icmp/  packet/       rxrpc/   sysctl_net.c  x25/

```

we see all source directory `net`, now we need config file [Makefile](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/blob/master/net/Makefile#L13) and [Kconfig](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/blob/master/net/Kconfig#L90),

Now we should define function name of file header [netfilter_icmp.c](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/blob/master/include/net/netfilter_icmp/netfilter_icmp.h)

```
root@raminfp /tmp/linux-4.8.0/include# cd net/
root@raminfp /tmp/linux-4.8.0/include//net# ls
6lowpan.h        cls_cgroup.h    flow_dissector.h         inetpeer.h            llc.h             net_ratelimit.h  sock.h
9p/              codel.h         flow.h                   inet_sock.h           llc_if.h          netrom.h         sock_reuseport.h
act_api.h        codel_impl.h    fou.h                    inet_timewait_sock.h  llc_pdu.h         nexthop.h        Space.h
addrconf.h       codel_qdisc.h   fq.h                     ip6_checksum.h        llc_s_ac.h        nfc/             stp.h
af_ieee802154.h  compat.h        fq_impl.h                ip6_fib.h             llc_sap.h         nl802154.h       switchdev.h
af_rxrpc.h       datalink.h      garp.h                   ip6_route.h           llc_s_ev.h        p8022.h          tc_act/
af_unix.h        dcbevent.h      genetlink.h              ip6_tunnel.h          llc_s_st.h        phonet/          tcp.h
af_vsock.h       dcbnl.h         geneve.h                 ipcomp.h              lwtunnel.h        ping.h           tcp_states.h
ah.h             devlink.h       gen_stats.h              ipconfig.h            mac80211.h        pkt_cls.h        timewait_sock.h
arp.h            dn_dev.h        gre.h                    ip_fib.h              mac802154.h       pkt_sched.h      transp_v6.h
atmclip.h        dn_fib.h        gro_cells.h              ip.h                  mip6.h            protocol.h       tso.h
ax25.h           dn.h            gtp.h                    ip_tunnels.h          mld.h             psnap.h          udp.h
ax88796.h        dn_neigh.h      gue.h                    ipv6.h                mpls.h            raw.h            udplite.h
bluetooth/       dn_nsp.h        hwbm.h                   ip_vs.h               mpls_iptunnel.h   rawv6.h          udp_tunnel.h
bond_3ad.h       dn_route.h      icmp.h                   ipx.h                 mrp.h             red.h            vsock_addr.h
bond_alb.h       dsa.h           ieee80211_radiotap.h     irda/                 ncsi.h            regulatory.h     vxlan.h
bonding.h        dsfield.h       ieee802154_netdev.h      iucv/                 ndisc.h           request_sock.h   wext.h
bond_options.h   dst_cache.h     if_inet6.h               iw_handler.h          neighbour.h       rose.h           wimax.h
busy_poll.h      dst.h           ila.h                    kcm.h                 netevent.h        route.h          x25device.h
caif/            dst_metadata.h  inet6_connection_sock.h  l3mdev.h              netfilter/        rtnetlink.h      x25.h
calipso.h        dst_ops.h       inet6_hashtables.h       lapb.h                netfilter_icmp/   sch_generic.h    xfrm.h
cfg80211.h       esp.h           inet_common.h            lib80211.h            netlabel.h        scm.h
cfg80211-wext.h  ethoc.h         inet_connection_sock.h   llc_c_ac.h            netlink.h         sctp/
cfg802154.h      fib_rules.h     inet_ecn.h               llc_c_ev.h            net_namespace.h   secure_seq.h
checksum.h       firewire.h      inet_frag.h              llc_conn.h            netns/            slhc_vj.h
cipso_ipv4.h     flowcache.h     inet_hashtables.h        llc_c_st.h            netprio_cgroup.h  snmp.h
root@raminfp /tmp/linux-4.8.0/include/net# cd netfilter_icmp/
root@raminfp /tmp/linux-4.8.0/include/net/netfilter_icmp# ls
netfilter_icmp.h
```

it's done,now we should define this function name from file header netfilter_icmp on main.c file linux,

go to :

```
root@raminfp /tmp/linux-4.8.0# cd init/
root@raminfp /tmp/linux-4.8.0/init# ls
calibrate.c  do_mounts.h         do_mounts_md.c  initramfs.c  Kconfig  Makefile       version.c
do_mounts.c  do_mounts_initrd.c  do_mounts_rd.c  init_task.c  main.c   noinitramfs.c
```

we declare header file [init/main.c](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/blob/master/init/main.c#L91&L93)
and call function header file [here](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/blob/master/init/main.c#L655&L657)


Building linux kernel :

```
make menuconfig
```

![kernel](http://up2www.com/uploads/a71d1.png "kernel")

Help `NETFILTER ICMP`


![kernel](http://up2www.com/uploads/2b5e2.png "kernel")

Now i bulding my code on the linux kernel you find binary package of build [here](https://github.com/raminfp/linux-4.8.0-netfilter_icmp/tree/master/build) 

build and install kernel package :
```
root@raminfp /tmp# cd linux-4.8.0/
root@raminfp /tmp/linux-4.8.0# ls
arch/   COPYING  debian.master/  firmware/  ipc/     lib/         net/            samples/        sound/   usr/
block/  CREDITS  Documentation/  fs/        Kbuild   MAINTAINERS  README          scripts/        spl/     virt/
build/  crypto/  drivers/        include/   Kconfig  Makefile     README.md       security/       tools/   zfs/
certs/  debian/  dropped.txt     init/      kernel/  mm/          REPORTING-BUGS  snapcraft.yaml  ubuntu/
root@raminfp /tmp/linux-4.8.0# cd build/
root@raminfp /tmp/linux-4.8.0/build# ls
linux-headers-4.8.17-netfilter-icmp_05-mar-2017_amd64.deb  linux-image-4.8.17-netfilter-icmp_05-mar-2017_amd64.deb
root@raminfp /tmp/linux-4.8.0/build# dpkg --install linux-headers-4.8.17-netfilter-icmp_05-mar-2017_amd64.deb
root@raminfp /tmp/linux-4.8.0/build# dpkg --install linux-image-4.8.17-netfilter-icmp_05-mar-2017_amd64.deb
root@raminfp /tmp/linux-4.8.0/build# reboot
```

Go to advance options of ubuntu :

![kernel](http://up2www.com/uploads/4382IMG-20170330-001911.jpg "boot")

now we see 4.8.17-netfilter_icmp this is we kernel,


![kernel](http://up2www.com/uploads/4382IMG-20170330-001941.jpg "boot")

We have on own kernel now what heppen of my code , please see image, we see kernel release and access to google.com,this means any drop packet ICMP,

![kernel](http://up2www.com/uploads/ab743.png "kernel")

now `echo 1 > /proc/netfilter_proc_a` we see `/proc/netfilter_proc_a` is `true` this means drop ICMP packet,

![kernel](http://up2www.com/uploads/ab744.png "kernel")
 

#### Special thanks to Kiran Kankipati 

