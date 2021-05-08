
void PackPacket(void)
{
	static uint16_t packetcounter=0;
	uint8_t packet[16];
	
	double longitude=(double)lon/1e7;
	double latitude=(double)lat/1e7;
	double hght=(double)hMSL/1e3;
	
	uint32_t packed_longitude=(uint32_t)(longitude*131072.0);
	uint32_t packed_latitude=(uint32_t)(latitude*131072.0);
	int16_t packed_height=(int16_t)hght;
	
	int cnt;
	uint8_t numsats=0;
	
	for(cnt=0;cnt<MAX_CHANNELS;cnt++)
	{
		if(cno[cnt]>0)	numsats++;
	}
	
	packet[0]=LORA_ID;	// just an id value
	
	packet[1]=numsats;
	packet[1]|=(gpsFix&0x03)<<6;
	
	packet[2]=(lon&0x000000ff)>>0;
	packet[3]=(lon&0x0000ff00)>>8;
	packet[4]=(lon&0x00ff0000)>>16;
	packet[5]=(lon&0xff000000)>>24;
	
	packet[6]=(lat&0x000000ff)>>0;
	packet[7]=(lat&0x0000ff00)>>8;
	packet[8]=(lat&0x00ff0000)>>16;
	packet[9]=(lat&0xff000000)>>24;
	
	packet[10]=(packed_height&0x00ff)>>0;
	packet[11]=(packed_height&0xff00)>>8;
	
	// horizontal accuracy estimate from NAV-POSLLH message in units of 0.5m
	packet[12]=hAccValue;
	
	// battery voltage divided by 20 so 4250 would read as a 212 count
	packet[13]=batvolt;
	
	packet[14]=(packetcounter&0x00ff)>>0;
	packet[15]=(packetcounter&0xff00)>>8;
	
	memcpy(TXPacket,packet,16);
	TxPacketLength=16;
	
	packetcounter++;
}

