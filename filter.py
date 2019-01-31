import os
pcapDir = '/home/soumya/ns3/'
for readFile in [f for f in os.listdir(pcapDir) if f.endswith('.pcap')]:
	writeFile = readFile[4:9].replace('_','')+'.txt'
	print 'tshark -r '+readFile+' -Y udp >>'+writeFile
	os.system('echo _________________'+readFile[8:-5]+' >>'+writeFile)
	os.system('tshark -r '+readFile+' -Y udp >>'+writeFile)
	