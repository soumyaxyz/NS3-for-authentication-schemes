import xml.etree.ElementTree as ET
import pdb; 
import os
allfiles 	= os.listdir('.')
summary_file = open("summary.csv", "w")
for i in xrange(len(allfiles)):
	if allfiles[i].endswith('flowMonitor.xml'):
		maxTime 		= 0;
		minTime 		= float('inf');
		totalDelay 		= 0
		totalReceived 	= 0
		messageCount	= 0
		tr =[]
		tree 			= ET.parse(allfiles[i])
		root 			= tree.getroot()
		for child in root:
			if child.tag == 'FlowStats':
				messageCount = len(child)
				for flow in child:
					#pdb.set_trace()
					flowDetails =  flow.attrib
					sent 		= int(flowDetails['timeFirstTxPacket'][1:-4])
					recv 		= int(flowDetails['timeLastRxPacket'][1:-4])
					if sent < minTime:
						minTime = sent
					if recv > maxTime:
						maxTime = recv
					totalDelay 		= totalDelay + int(flowDetails['delaySum'][1:-4])
					totalReceived	= totalReceived + int(flowDetails['rxBytes'])
					tr.append(int(flowDetails['rxPackets']))
		
		#print allfiles[i]
		#print totalReceived
		#print totalDelay
		#print maxTime - minTime , maxTime , minTime
		# maxTime / 1000000000.0


		#print totalReceived / ((maxTime - minTime)/ 1000000000.0)
		#print (totalDelay/1000000.0) / messageCount

		#pdb.set_trace()
		pdr = round( sum(tr)*1.0/messageCount, 2)

		nodeCounts 		= str(allfiles[i].replace('IOT_','').replace('__flowMonitor.xml',', ').replace('x',', ') )
		throughput 		= str( round( totalReceived / ((maxTime - minTime)/ 1000000000.0) ,3) )		# in bytes per second
		end2endDelay 	= str( round( (totalDelay/1000000000.0) / messageCount ,3) )  					# in miliseconds
		print nodeCounts +'messageCount = '+ str(messageCount) +', recvd = '+ str(totalReceived) +', time = '+ str(round(totalDelay/1000000000.0,3) )+ ', throughput =' +  throughput+ ', end2endDelay (sec) = '+ end2endDelay+'\n'
		output =  nodeCounts + ', ' + str(messageCount) +', '+str(totalReceived)+ ', '+ str(round(totalDelay/1000000000.0,3) )+ ', '+str(round((maxTime - minTime)/1000000000.0,3) )+ ', '+ throughput + ', ' + end2endDelay  + ', ' + str(pdr)+"\n"
		
		#print output
		#pdb.set_trace()
		#print sum(tr)
		#print messageCount
		#print pdr
		#print round(maxTime / 1000000000.0, 3)
		summary_file.write(output)



