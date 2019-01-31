#pcapDir = '/mnt/d/pcap'
#for readFile in [f for f in os.listdir(pcapDir) if f.endswith('.txt')]:


class Msg:
	def __init__(self,src,dest,time,size):
		self.src	= src
		self.dest	= dest
		self.time	= time
		self.size	= size		
	def check_reply(self,src,dest):
		if int(self.src.replace('.','')) == int(dest.replace('.','')):
			if int(self.dest.replace('.','')) == int(src.replace('.','')):
				return true
		return false











import os
allfiles 	= os.listdir('.')
for i in xrange(len(allfiles)):
	if allfiles[i].endswith('.txt'):
		lines 		= open(allfiles[i],'r').readlines()
		#d 			= dict()
		#e2e			= []
		#totalbytes	= 0
		maxtime 	= 0
		for line in lines:
			#print line
			parts = line.replace('  ',' ').replace('  ',' ').split(' ')[2:]
			if len(parts)>2:
				#item 		= parts[2].replace('.','')+parts[4].replace('.','')
				#totalbytes = totalbytes + (int(parts[7])-64)
				if parts[0] > maxtime :
					maxtime = parts[0]
		print allfiles[i].replace('.txt',',').replace('x',',')+str(maxtime)