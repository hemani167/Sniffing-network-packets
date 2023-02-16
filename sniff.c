
#include<stdio.h>	//For standard things
#include<stdlib.h>	//malloc
#include<string.h>	//memset
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<linux/if_ether.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>


int main()
{
	int sock_r;
 struct ifreq ifr;
 struct sockaddr_ll sall;
	sock_r=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sock_r<0)
	{
		printf("error in socket\n");
		return -1;
	}
  printf("Raw socket created\n");
	unsigned char *buffer = (unsigned char *) malloc(65536); //to receive data
	memset(buffer,0,65536);
  printf("allocate buffer\n");
  
//	struct sockaddr saddr;
	//int saddr_len = sizeof (saddr);
 
 // Get the index of the interface
    strncpy(ifr.ifr_name, "enp96s0f1", IFNAMSIZ);
    if (ioctl(sock_r, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        exit(1);
    }
     printf("get index\n");
 // Bind the socket to the interface
    sall.sll_family = AF_PACKET;
    sall.sll_ifindex = ifr.ifr_ifindex;
    sall.sll_protocol = htons(ETH_P_ALL);
    if (bind(sock_r, (struct sockaddr *)&sall, sizeof(sall)) < 0) {
        perror("bind");
        exit(1);
    }
 printf("Binding done!!!\n");
	 
	//Receive a network packet and copy in to buffer
 int t=1;
 while(t){
  	int buflen=recvfrom(sock_r,buffer,65536,0,NULL,NULL);
  	if(buflen<0)
  	{
  		printf("error in reading recvfrom function\n");
  		return -1;
  	}
   printf("printing buffer: %s \n",buffer);
   printf("printing buffer: %.2x \n",buffer);
  	struct ethhdr *eth = (struct ethhdr *)buffer;
   
    
    
//  	printf("\nEthernet Header\n");
//  	printf("Source Address : %.2x %.2x %.2x %.2x %.2x %.2x\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
//  	printf("Destination Address : %.2x-%.2x-%.2x-%.2x-%.2x-%.2x\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
//  	printf("Protocol : %d\n",eth->h_proto);
 
 //printf("**********************************************************\n");

struct ether_header *eth_header = (struct ether_header *)buffer;
        unsigned char *data = buffer ;
        //sizeof(struct ether_header);
        int data_len = buflen - sizeof(struct ether_header);

        // Print the source and destination MAC addresses
        printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                eth_header->ether_shost[0], eth_header->ether_shost[1],
                eth_header->ether_shost[2], eth_header->ether_shost[3],
                eth_header->ether_shost[4], eth_header->ether_shost[5]);
        printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
                eth_header->ether_dhost[0], eth_header->ether_dhost[1],
                eth_header->ether_dhost[2], eth_header->ether_dhost[3],
                eth_header->ether_dhost[4], eth_header->ether_dhost[5]);
        printf("Protocol: 0x%04x\n", ntohs(eth_header->ether_type));
        printf("Data: %s\n", data);
    


//    //Extracting data
//    data = (buffer + sizeof(struct ethhdr));
//    
    int remaining_data = buflen - (sizeof(struct ether_header));
    printf("DATA:");
    for(int i=0;i<remaining_data;i++)
    {
      if(i!=0 && i%16==0)
      printf("\n");
      printf("%.2X ",data[i]);
    }
//t--;
printf("\n**********************************************************\n");
}
	close(sock_r);
	printf("finished");
 return 0;
}


