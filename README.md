#NS3
<!DOCTYPE html>
<html>
<body class="stackedit">
  <div class="stackedit__html"><h1 id="NS3-simulation-for-authentication-schemes-a-walkthrough">NS3 simulation for authentication schemes: a walkthrough</h1>
<p>In this writeup, we will model an authentication mechanism for IoT devices.  A NS3 model is insutable for security analysis. Thus, we are not attempting to analyse the security aspect of the scheme, we only intend to measure the network  impact  (end-to-end delay, throughput, packet delivery ratio etc) of such an scheme in a deployment setting.</p>
<p>A high level description of the scheme in question:<br>
<a href="https://i.stack.imgur.com/osMTA.png"><img src="https://i.stack.imgur.com/osMTA.png" alt="enter image description here"></a></p>
<p>For our NS3 simulation we need to know the sizes  M<sub>1</sub> - M<sub>3</sub> . In the scheme in question</p>
<ul>
<li>M<sub>1</sub> = 104 Bytes</li>
<li>M<sub>2</sub> = 84 Bytes</li>
<li>M<sub>3</sub> = 84 Bytes</li>
</ul>
<h2 id="ns3-installation-and-setup">NS3 installation and setup</h2>
<p>The <a href="https://www.nsnam.org/wiki/Installation">installation guide</a> at <a href="http://nsnam.org">nsnam.org</a> is very detailed. It is also highly recommended that new NS3 users go through the <a href="https://www.nsnam.org/docs/release/3.29/tutorial/html/index.html">tutorial</a>. The website is the official repository for NS3 and the most reliable resource. In case of issues, the <a href="https://groups.google.com/forum/#!forum/ns-3-users">ns3-user google group</a> is the most active forum for NS3. The maintainers of the projects are active there and oftentimes are there to  guide newcomers.</p>
<h2 id="ns3-simulation-code">NS3 simulation code</h2>
<p>We can model NS3 simulations in C++ or python. In my experience,  as the examples provided are mostly in C++, its more convenient to write the code in C++ purely for the relatively more abundant reference material.<br>
The C++ code is written under the ns3 namespace. We also set up some global defaults.</p>
<pre><code>#include &lt;stdlib.h&gt;    					 	// for rand()
#include &lt;math.h&gt;							// for mathemetical operations
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"
#include "ns3/basic-energy-source.h"
#include "ns3/flow-monitor-helper.h"
//#include "ns3/simple-device-energy-model.h"
//#include "ns3/v4ping-helper.h"
//#include "ns3/v4ping.h"

//using namespace ns3;

//static bool verbose = 0;
//uint32_t M1 = 104, M2=84, M3 = 84;

//char * stringbuilder( char* prefix,  char* sufix){
 // char* buf = (char*)malloc(50); 
 // snprintf(buf, 50, "%s%s", prefix, sufix);
  return  buf;
}
</code></pre>
<p>We begin the program as a normal  C++ program  and define variables to be set up by the command line argument</p>
<pre><code>int main (int argc, char *argv[])
{
  
  //
  // First, we declare and initialize a few local variables that control some
  // simulation parameters.
  
  
  uint32_t mobileUserNodes = 3;
  uint32_t smartDeviceNodes = 2;
  uint32_t stopTime = 3600;
  bool verbose = 0;
  bool enablePcap = 0;
  bool enableAnim = 0;
  bool verifyResults = 0; //used for regression
  char saveFilePrefix[50] ;
  

  //
  // Simulation defaults are typically set next, before command line
  // arguments are parsed.
  //
  //
  //
  // For convenience, we add the local variables to the command line argument
  // system so that they can be overridden with flags such as
  // "--smartDeviceNodes=20"
  //
  CommandLine cmd;
  
  cmd.AddValue ("MU", "number of User nodes", mobileUserNodes);
  cmd.AddValue ("SD", "number of smart device nodes", smartDeviceNodes);
  cmd.AddValue ("t", "simulation stop time (seconds)", stopTime);  
  cmd.AddValue ("p", "Enable/disable pcap file generation", enablePcap);
  cmd.AddValue ("a", "Enable/disable xml gneration for netanim-module", enableAnim);
  cmd.AddValue ("o", "Show output end of the simulation", verifyResults);
  cmd.AddValue ("v", "Verbose mode.", verbose);
  cmd.AddValue ("s", "Define the prefix for .pcap anf .xml files. Default: IOT ", saveFilePrefix);
  cmd.AddValue ("M1", "Size of message 1 ", M1);
  cmd.AddValue ("M2", "Size of message 2 ", M2);
  cmd.AddValue ("M3", "Size of message 3 ", M3);


  

  //
  // The system global variables and the local values added to the argument
  // system can be overridden by command line arguments by using this call.
  //
  cmd.Parse (argc, argv);

  if (stopTime &lt; 2)
    {
      std::cout &lt;&lt; "Use a simulation stop time &gt;= 2 seconds" &lt;&lt; std::endl;
      exit (1);
    }  

if (verbose)
  {
    //LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
    //LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
    LogComponentEnable("Simulator", LOG_LEVEL_INFO);
  }
</code></pre>
<p>The logging module can be very useful  for debugging  simulations. This cleaned up code does not demonstrate the use of logging. Interested readers may refer to <a href="https://www.nsnam.org/docs/release/3.29/tutorial/html/tweaking.html#using-the-logging-module">this section</a> of the previously referenced NS3 tutorial.</p>
<p>This write up assumes that the reader has already followed the NS3 tutorial and is familiar with the basics. Thus without elaborating we state that ve need distinct nodes for  each role in the simulation.  <em>mobileUserNodes</em> and  <em>smartDeviceNodes</em> account for the number of the users and smart devices in the simulation. There will be a single gateway node. <em>M<sub>1</sub></em>,  <em>M<sub>2</sub></em> and   <em>M<sub>3</sub></em> defines the sizes of the three authentication messages. <em>stopTime</em> defines when the simulation will stop. By default the code shall run the simulation for  3600 seconds or half an hour. Note that this is half hour of simulated network runtime, not how long it will take to actually run the simulation. As NS3 is a discrete event simulator it completely depends on the complexity of the protocol modelled.</p>
<pre><code>  //Since default reference loss is defined for 5 GHz, it needs to be changed when operating at 2.4 GHz
  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceLoss", DoubleValue (40.046));
  
  // creating nodes
  //NodeContainer allNodes;
  NodeContainer wifiUserNodes;
  wifiUserNodes.Create (mobileUserNodes);
  //allNodes.Add (wifiUserNodes);
  NodeContainer wifiDeviceNodes;
  wifiDeviceNodes.Create (smartDeviceNodes);
  //allNodes.Add (wifiDeviceNodes);
  NodeContainer wifiGateway ;
  wifiGateway.Create (1);
  //allNodes.Add (wifiGateway);

  // creating wireless channel
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer UserDevices;
  UserDevices = wifi.Install (phy, mac, wifiUserNodes);
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer SmartDevices;
  SmartDevices = wifi.Install (phy, mac, wifiDeviceNodes);
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));


  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiGateway);

</code></pre>
<p>We  set up the simulation such that the users, gateway and the smart devices communicate over the 2.4 GHz wi-fi networks. To this goal, we first set up ‘NodeContainer’ to separate hold the nodes for  the users, gateway and the smart devices. Then, we define a wireless channel and its properties.<br>
<code>YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();</code><br>
<code>YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();</code><br>
<code>phy.SetChannel (channel.Create ());</code><br>
Then we define <code>WifiHelper</code> and <code>WifiMacHelpers</code> necessary for defining the actual wifi hardware  . Now, we can set up the <code>NetDeviceContainer</code>s  corresponding to the different nodes. Note that the gateway node is set up as a wifi Access point and the remaining are set up as wifi devices.</p>
<pre><code>
  // defining Mobility

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (-10.0),
                                 "MinY", DoubleValue (-10.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (5.0),
                                 "GridWidth", UintegerValue (5),
                                 "LayoutType", StringValue ("RowFirst"));
  
  mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-150, 150, -150, 150)),
                                 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=3]"),
                                 "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.4]"));
  mobility.Install (wifiUserNodes);

 
  Ptr&lt;ListPositionAllocator&gt; subnetAlloc =   CreateObject&lt;ListPositionAllocator&gt; ();
  subnetAlloc-&gt;Add (Vector (0.0, 0.0, 0.0));   //for gateway
  for (uint32_t j = 0; j &lt; wifiDeviceNodes.GetN (); ++j){
    double  theta = (j)*360/wifiDeviceNodes.GetN();
    uint32_t  r =((double)rand() / (RAND_MAX))*80 +20;
    subnetAlloc-&gt;Add (Vector (sin(theta)*r, cos(theta)*r, 0.0));
    //std::cout &lt;&lt;"[ "&lt;&lt;sin(theta)*r&lt;&lt;","&lt;&lt; cos(theta)*r&lt;&lt;","&lt;&lt; theta&lt;&lt;"]"&lt;&lt;"r = "&lt;&lt;r&lt;&lt;std::endl;
  }
  mobility.SetPositionAllocator (subnetAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiGateway);
  mobility.Install (wifiDeviceNodes);
</code></pre>
<p>We do not need to simulate energy requirements of the devices for authentication schemes, thus don’t need to define a  energy model but we need to simulate the specify the position of the different nodes in order to simulate the presence of the virtual network devices. Thus we define the mobility model for the nodes. We define two mobility models:</p>
<ul>
<li>a <code>ConstantPositionMobilityModel</code>for the static gateway and smart devices</li>
<li>and a <code>RandomDirection2dMobilityModel</code> for the users, who can randomly move  in any direction with speeds upto 3m’s and within a 150 m<sup>2</sup> square centered on the origin.</li>
</ul>
<p>We also need to define the starting position for the nodes. We utilize the  <code>GridPositionAllocator</code> to set position of the users . But, for the rest we  use a <code>ListPositionAllocator</code>  to  set the gateway at origin and randomly scatter the smartdevices across a  80 m ring with inner radius 20 m and centered on the origin.</p>
<pre><code>

  // Installing internet stack

  InternetStackHelper stack;
  OlsrHelper olsr;
  stack.SetRoutingHelper (olsr); 
  stack.Install (wifiUserNodes);
  stack.Install (wifiDeviceNodes);
  stack.Install (wifiGateway);
  // Install Ipv4 addresses

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterface;
  apInterface = address.Assign (apDevices);
  //Ipv4InterfaceContainer userInterfaces;
  apInterface = address.Assign (UserDevices);
  //Ipv4InterfaceContainer deviceInterfaces;
  apInterface = address.Assign (SmartDevices);


  // crating applications
  ApplicationContainer serverAppContainer, clientAppContainer;  
  uint16_t port = 9;  // well-known echo port number
  Ptr&lt;Node&gt; gateway = wifiGateway.Get (0);
  UdpServerHelper server(port);
  serverAppContainer.Add(server.Install (gateway));


double time = 1;
for (uint32_t i = 0; i &lt; wifiUserNodes.GetN (); ++i){
	Ptr&lt;Node&gt; user = wifiUserNodes.Get (i);
	for (uint32_t j = 0; j &lt; wifiDeviceNodes.GetN (); ++j){
		Ptr&lt;Node&gt; device = wifiDeviceNodes.Get (j);
    if(i==0){
      serverAppContainer.Add(server.Install (device));
      //std::cout &lt;&lt;"device "&lt;&lt;j&lt;&lt;std::endl;
    }
    clientAppContainer = authenticate(clientAppContainer, time , user, gateway , device ); 
    //time = time +.2;   
    //std::cout &lt;&lt;time&lt;&lt;std::endl;
	}
  serverAppContainer.Add(server.Install (user));
  //time = time +.1;
  //std::cout &lt;&lt;"user "&lt;&lt;i&lt;&lt;std::endl;
}



  serverAppContainer.Start (Seconds (0.0));
  serverAppContainer.Stop (Seconds (stopTime+1));
  


  //clientAppContainer.Start (Seconds (1.0));   //started induvugualy
  clientAppContainer.Stop (Seconds (stopTime+1));





</code></pre>
<p>Now, we set-up the internet stack for  the <code>NodeContainer</code>s  previously defined. We specify the use of OLSR routing. We also associate a <code>Ipv4InterfaceContainer</code> with the  previously defined<code>NetDeviceContainer</code> to assign them IPV4 addresses.</p>
<p>Now at last, we can define the applications.  We use Udp server and clients as our applications. Each users, smart devices and  gateway node is modeled with a <code>UdpClient</code> to send the authentication messages and a <code>UdpServer</code> to receive the response.  First we  setup two <code>ApplicationContainer</code>: <code>serverAppContainer</code> and  <code>clientAppContainer</code> to hold all the udp servers and clients at once. Then we describe a prototype  udp server with the help of <code>UdpServerHelper</code>.  A udp client in NS3 can repeatedly at fixed interval  send a packet of data of predefined size.  We could extended the <code>UdpServer</code> and <code>UdpClient</code> to define our own server and client but since we are not attempting to provide actual authentication or prove its correctness (which must be done separately) , we can merely user equal sized  data packets for our simulation. We create a udp client to send a single message, for every message.</p>
<pre><code>ApplicationContainer authenticate(ApplicationContainer appContainer, double time, Ptr&lt;Node&gt; user, Ptr&lt;Node&gt; gateway , Ptr&lt;Node&gt; device ){	

  if (verbose){
    std::cout&lt;&lt;"user : "&lt;&lt; user-&gt;GetObject&lt;Ipv4&gt; ()-&gt;GetAddress (1, 0).GetLocal ();
    std::cout&lt;&lt;"    gateway : "&lt;&lt; gateway-&gt;GetObject&lt;Ipv4&gt; ()-&gt;GetAddress (1, 0).GetLocal ();
	  std::cout&lt;&lt;"    device : "&lt;&lt; device-&gt;GetObject&lt;Ipv4&gt; ()-&gt;GetAddress (1, 0).GetLocal ()&lt;&lt;std::endl;
  }
	
	appContainer = sendMessage(appContainer, time, user, device , M1);
	appContainer = sendMessage(appContainer, time, gateway, device,  M2); 
	appContainer = sendMessage(appContainer, time, device, user, M3); 

  return appContainer;
}

</code></pre>
<pre><code>ApplicationContainer sendMessage(ApplicationContainer apps, double time, Ptr&lt;Node&gt;source,Ptr&lt;Node&gt;sink, uint32_t packetSize){
    Ipv4Address  remoteAddress = sink-&gt;GetObject&lt;Ipv4&gt; ()-&gt;GetAddress (1, 0).GetLocal ();
 	  
  uint16_t port = 9;  // well-known echo port number
  Time interPacketInterval = Seconds (20.);
  UdpClientHelper client (remoteAddress, port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  client.SetAttribute ("StartTime", TimeValue (Seconds (time)));
  //std::cout&lt;&lt;time&lt;&lt;"&gt;&gt;";
  apps.Add(client.Install (source));
  return apps;
 }
</code></pre>
<p>The <code>sendMessage()</code>function creates the creates the  individual <code>UdpClients</code> for each messages. The <code>authenticate()</code> function  simulates a single authentication handshake, thus invokes <code>sendMessage()</code> thrice. Consequently,<code>authenticate()</code>need to be separately invoked for distinct each set of <em>&lt;user, gateway , device &gt;</em>. The variable time passed to <code>sendMessage()</code> schedules when each message is to be sent.  If too much congestion is observed in the result, this value can be used to stagger the messages (see commented code). Then we set the start and stop time for all application at once using the <code>serverAppContainer</code> and the <code>clientAppContainer</code>.</p>
<pre><code>if (verbose){
  std::cout &lt;&lt;"servers stops at  "&lt;&lt;stopTime+1&lt;&lt;std::endl;
  std::cout &lt;&lt;"final transmission  scheduled at  "&lt;&lt;(time-.33)&lt;&lt;std::endl;

  std::cout &lt;&lt; "server apps installed till now :"&lt;&lt;serverAppContainer.GetN ()&lt;&lt; std::endl;
  std::cout &lt;&lt; "client apps installed till now :"&lt;&lt;clientAppContainer.GetN ()&lt;&lt; std::endl;
}

  snprintf(saveFilePrefix, 50, "IOT_%dx%d_", mobileUserNodes, smartDeviceNodes);

if (enablePcap){
	phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_users"), UserDevices, 0);
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_devices"), SmartDevices, 0);
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_gateway"), apDevices, 0);

}


if(enableAnim) {
  AnimationInterface anim (stringbuilder(saveFilePrefix,(char*)"-animation.xml")); // Mandatory
  for (uint32_t i = 0; i &lt; wifiUserNodes.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiUserNodes.Get (i), "MU"); // Optional
      anim.UpdateNodeColor (wifiUserNodes.Get (i), 255, 0, 0); // Optional
    }
  for (uint32_t i = 0; i &lt; wifiDeviceNodes.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiDeviceNodes.Get (i), "SD"); // Optional
      anim.UpdateNodeColor (wifiDeviceNodes.Get (i), 255, 255, 0); // Optional
    }
  for (uint32_t i = 0; i &lt; wifiGateway.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiGateway.Get (i), "Gateway"); // Optional
      anim.UpdateNodeColor (wifiGateway.Get (i), 0, 255, 0); // Optional
    }
  //anim.EnablePacketMetadata (); // Optional/
  anim.EnableWifiMacCounters (Seconds (0), Seconds (10)); //Optional
  anim.EnableWifiPhyCounters (Seconds (0), Seconds (10)); //Optional
}

</code></pre>
<p>The above snippet of code, defines how (if the option is set) pcap files are to be saved and how (if the option is set) a xml file can be generated to be viewed in <a href="https://www.nsnam.org/wiki/NetAnim">NetAnim</a>.</p>
<pre><code>

  //Populate routing table
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  
  // setting up flowMonitor
  
  Ptr&lt;FlowMonitor&gt; flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  // setting up simulator

  Simulator::Stop (Seconds (stopTime+1));
  Simulator::Run ();
  Simulator::Destroy ();
  flowMonitor-&gt;SerializeToXmlFile(stringbuilder(saveFilePrefix,(char*)"_flowMonitor.xml"), false, false);

</code></pre>
<p>The  <code>flowMonitor</code> is utilized to get the transmitted data form the simulation. NS3 can produce  trace(<em>.tr</em>) files  and <em>.pcap</em> files (as shown above) but using the *.xml * generated through <code>flowMonitor</code> the useful data can be extracted with much less hassle.</p>
<pre><code>
  uint32_t bytes_received = 0;

  for (uint32_t i = 0; i &lt; serverAppContainer.GetN (); ++i){
    char nodename[30+sizeof(serverAppContainer.GetN ())*8];
    uint32_t expected = 1;
    uint32_t totalPacketsThrough = DynamicCast&lt;UdpServer&gt; (serverAppContainer.Get (i))-&gt;GetReceived ();;
    if (i==0){
      snprintf(nodename, sizeof(nodename), "gateway\t");
      expected = M2;//martDeviceNodes * mobileUserNodes;
      bytes_received += expected;
    }else if (i&lt;smartDeviceNodes+1){
      snprintf(nodename, sizeof(nodename), "smart device %d ", i);
      expected = mobileUserNodes *M1;
      bytes_received += expected;
    }else{
      snprintf(nodename, sizeof(nodename), "mobile user %d ", i-smartDeviceNodes);
      expected = smartDeviceNodes*M3;
      bytes_received += expected;
    } 

    
    std::cout &lt;&lt;"Number of bytes received at "&lt;&lt;nodename&lt;&lt;"\t : " &lt;&lt; totalPacketsThrough &lt;&lt;" / "&lt;&lt; expected &lt;&lt; std::endl;
    //uint32_t lost =  DynamicCast&lt;UdpServer&gt; (serverAppContainer.Get (i))-&gt;GetLost ();
    //uint32_t window =  DynamicCast&lt;UdpServer&gt; (serverAppContainer.Get (i))-&gt;GetPacketWindowSize ();
    //std::cout &lt;&lt;"\t. Packets lost: " &lt;&lt; lost &lt;&lt;"( "&lt;&lt;window&lt;&lt;" ) "&lt;&lt; std::endl;
  }

  std::cout &lt;&lt;"Total bytes received ("&lt;&lt;mobileUserNodes&lt;&lt;" , "&lt;&lt;smartDeviceNodes&lt;&lt;") : "&lt;&lt; bytes_received &lt;&lt; std::endl;
  return 0;
}
</code></pre>
<p>The closing bit of code for our NS3 simulation. This is completely optional and merely shows some statistics after simulation completion.</p>
<h2 id="running-the-ns3-simulation">Running the NS3 simulation</h2>
<p>We can run the simulation with the following line in the terminal<br>
<code>./waf --run "IOT_PUF --MU=10 --SD=20"</code></p>
<p>Where <strong>IOT_PUF.cc</strong> is the NS3 simulation code and is saved in the <code>scratch/</code> directory. On successful execution it will generate  <strong>IOT_10_20.xml</strong> file as output.</p>
<p>We can use a shell script to schedule the sequential run of several simulations with different parameters</p>
<pre><code>./waf --run "IOT_PUF --MU=5 --SD=20
./waf --run "IOT_PUF --MU=10 --SD=20
./waf --run "IOT_PUF --MU=15 --SD=20

python xmlParser.py
</code></pre>
<h2 id="parsing-the-output-xml">Parsing the output XML</h2>
<p>We utilize a small python script to extract  information from one or more  output XML files at once.  This script is written to  parse the output of the different scenarios of the same  simulation, at once.</p>
<pre><code>import xml.etree.ElementTree as ET
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
					flowDetails =  flow.attrib
					sent 		= int(flowDetails['timeFirstTxPacket'][1:-4])
					recv 		= int(flowDetails['timeLastRxPacket'][1:-4])
					if sent &lt; minTime:
						minTime = sent
					if recv &gt; maxTime:
						maxTime = recv
					totalDelay 		= totalDelay + int(flowDetails['delaySum'][1:-4])
					totalReceived	= totalReceived + int(flowDetails['rxBytes'])
					tr.append(int(flowDetails['rxPackets']))
		
		
		pdr = round( sum(tr)*1.0/messageCount, 2)  				#  package deliverry ratio
		nodeCounts 		= str(allfiles[i].replace('IOT_','').replace('__flowMonitor.xml',', ').replace('x',', ') )
		throughput 		= str( round( totalReceived / ((maxTime - minTime)/ 1000000000.0) ,3) )		# in bytes per second
		end2endDelay 	= str( round( (totalDelay/1000000000.0) / messageCount ,3) )  					# in miliseconds
		print nodeCounts +'messageCount = '+ str(messageCount) +', recvd = '+ str(totalReceived) +', time = '+ str(round(totalDelay/1000000000.0,3) )+ ', throughput =' +  throughput+ ', end2endDelay (sec) = '+ end2endDelay+'\n'
		output =  nodeCounts + ', ' + str(messageCount) +', '+str(totalReceived)+ ', '+ str(round(totalDelay/1000000000.0,3) )+ ', '+str(round((maxTime - minTime)/1000000000.0,3) )+ ', '+ throughput + ', ' + end2endDelay  + ', ' + str(pdr)+"\n"
		
		#print output		
		summary_file.write(output)
</code></pre>
<p>The script utilizes the <a href="https://docs.python.org/2/library/xml.etree.elementtree.html">ElementTree</a>  module of python to parse one xml  file at a time  to summarize the data into  <strong>summary.csv</strong> file.  Each row in the file corresponds to a scenario of the simulation.</p>
</div>
</body>

</html>
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTU0MDk2NjIzNl19
-->