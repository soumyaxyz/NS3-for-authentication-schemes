/* GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Soumya Banerjee (Adapted from wireless-animation.cc)
 */

#include <stdlib.h>     /* srand, rand */
#include <math.h>
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

using namespace ns3;


static bool verbose = 0;
uint32_t M1 = 104, M2=84, M3 = 84;

char * stringbuilder( char* prefix,  char* sufix){
  char* buf = (char*)malloc(50); 
  snprintf(buf, 50, "%s%s", prefix, sufix);
  return  buf;
}


ApplicationContainer sendMessage(ApplicationContainer apps, double time, Ptr<Node>source,Ptr<Node>sink, uint32_t packetSize){
    Ipv4Address  remoteAddress = sink->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
 	  
  uint16_t port = 9;  // well-known echo port number
  Time interPacketInterval = Seconds (20.);
  UdpClientHelper client (remoteAddress, port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  client.SetAttribute ("StartTime", TimeValue (Seconds (time)));
  //std::cout<<time<<">>";
  apps.Add(client.Install (source));
  return apps;
 }

ApplicationContainer authenticate(ApplicationContainer appContainer, double time, Ptr<Node> user, Ptr<Node> gateway , Ptr<Node> device ){	

  if (verbose){
    std::cout<<"user : "<< user->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    std::cout<<"    gateway : "<< gateway->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
	  std::cout<<"    device : "<< device->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ()<<std::endl;
  }
	
	appContainer = sendMessage(appContainer, time, user, device , M1);
	appContainer = sendMessage(appContainer, time, gateway, device,  M2); 
	appContainer = sendMessage(appContainer, time, device, user, M3); 

  //appContainer = sendMessage(appContainer, time, user, device , M1);
  //appContainer = sendMessage(appContainer, time, device, user, M2); 
  return appContainer;
}


int main (int argc, char *argv[])
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
  //cmd.AddValue ("backboneNodes", "number of backbone nodes", backboneNodes);
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

  if (stopTime < 2)
    {
      std::cout << "Use a simulation stop time >= 2 seconds" << std::endl;
      exit (1);
    }  

if (verbose)
  {
    //LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
    //LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
    LogComponentEnable("Simulator", LOG_LEVEL_INFO);
  }

  //Since default reference loss is defined for 5 GHz, it needs to be changed when operating at 2.4 GHz
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


  // defining Mobility

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (-10.0),
                                 "MinY", DoubleValue (-10.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (5.0),
                                 "GridWidth", UintegerValue (5),
                                 "LayoutType", StringValue ("RowFirst"));
  //mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
  //                           "Bounds", RectangleValue (Rectangle (-50, 50, -25, 50)));

  mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-150, 150, -150, 150)),
                                 "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=3]"),
                                 "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.4]"));
  mobility.Install (wifiUserNodes);

 
  Ptr<ListPositionAllocator> subnetAlloc =   CreateObject<ListPositionAllocator> ();
  subnetAlloc->Add (Vector (0.0, 0.0, 0.0));   //for gateway
  for (uint32_t j = 0; j < wifiDeviceNodes.GetN (); ++j){
    double  theta = (j)*360/wifiDeviceNodes.GetN();
    uint32_t  r =((double)rand() / (RAND_MAX))*80 +20;
    subnetAlloc->Add (Vector (sin(theta)*r, cos(theta)*r, 0.0));
    //std::cout <<"[ "<<sin(theta)*r<<","<< cos(theta)*r<<","<< theta<<"]"<<"r = "<<r<<std::endl;
  }
  mobility.SetPositionAllocator (subnetAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiGateway);
  mobility.Install (wifiDeviceNodes);




  // Installing internet stack

  InternetStackHelper stack;
  OlsrHelper olsr;
  stack.SetRoutingHelper (olsr); // has effect on the next Install ()??
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
  Ptr<Node> gateway = wifiGateway.Get (0);
  UdpServerHelper server(port);
  serverAppContainer.Add(server.Install (gateway));


double time = 1;
for (uint32_t i = 0; i < wifiUserNodes.GetN (); ++i){
	Ptr<Node> user = wifiUserNodes.Get (i);
	for (uint32_t j = 0; j < wifiDeviceNodes.GetN (); ++j){
		Ptr<Node> device = wifiDeviceNodes.Get (j);
    if(i==0){
      serverAppContainer.Add(server.Install (device));
      //std::cout <<"device "<<j<<std::endl;
    }
    clientAppContainer = authenticate(clientAppContainer, time , user, gateway , device ); 
    //time = time +.2;   
    //std::cout <<time<<std::endl;
	}
  serverAppContainer.Add(server.Install (user));
  //time = time +.1;
  //std::cout <<"user "<<i<<std::endl;
}



  serverAppContainer.Start (Seconds (0.0));
  serverAppContainer.Stop (Seconds (stopTime+1));
  


  //clientAppContainer.Start (Seconds (1.0));   //started induvugualy
  clientAppContainer.Stop (Seconds (stopTime+1));



  
 

if (verbose){
  std::cout <<"servers stops at  "<<stopTime+1<<std::endl;
  std::cout <<"final transmission  scheduled at  "<<(time-.33)<<std::endl;

  std::cout << "server apps installed till now :"<<serverAppContainer.GetN ()<< std::endl;
  std::cout << "client apps installed till now :"<<clientAppContainer.GetN ()<< std::endl;
}

  snprintf(saveFilePrefix, 50, "IOT_%dx%d_", mobileUserNodes, smartDeviceNodes);

if (enablePcap){
    //NS_LOG_INFO ("Configure Tracing.");
  
    //
    // Let's set up some ns-2-like ascii traces, using another helper class
    //
    //AsciiTraceHelper ascii;
    //Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (stringbuilder(saveFilePrefix,(char*)"_trace.tr"));
    //phy.EnableAsciiAll (stream);
    //stack.EnableAsciiIpv4All (stream);
  
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_users"), UserDevices, 0);
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_devices"), SmartDevices, 0);
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_gateway"), apDevices, 0);

}


if(enableAnim) {
  AnimationInterface anim (stringbuilder(saveFilePrefix,(char*)"-animation.xml")); // Mandatory
  for (uint32_t i = 0; i < wifiUserNodes.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiUserNodes.Get (i), "MU"); // Optional
      anim.UpdateNodeColor (wifiUserNodes.Get (i), 255, 0, 0); // Optional
    }
  for (uint32_t i = 0; i < wifiDeviceNodes.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiDeviceNodes.Get (i), "SD"); // Optional
      anim.UpdateNodeColor (wifiDeviceNodes.Get (i), 255, 255, 0); // Optional
    }
  for (uint32_t i = 0; i < wifiGateway.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiGateway.Get (i), "Gateway"); // Optional
      anim.UpdateNodeColor (wifiGateway.Get (i), 0, 255, 0); // Optional
    }
  //anim.EnablePacketMetadata (); // Optional/
  anim.EnableWifiMacCounters (Seconds (0), Seconds (10)); //Optional
  anim.EnableWifiPhyCounters (Seconds (0), Seconds (10)); //Optional
}




  //Populate routing table
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // setting up simulator
  

  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  Simulator::Stop (Seconds (stopTime+1));
  Simulator::Run ();
  Simulator::Destroy ();
  flowMonitor->SerializeToXmlFile(stringbuilder(saveFilePrefix,(char*)"_flowMonitor.xml"), false, false);




  uint32_t bytes_received = 0;

for (uint32_t i = 0; i < serverAppContainer.GetN (); ++i){
  char nodename[30+sizeof(serverAppContainer.GetN ())*8];
  uint32_t expected = 1;
  uint32_t totalPacketsThrough = DynamicCast<UdpServer> (serverAppContainer.Get (i))->GetReceived ();;
  if (i==0){
    snprintf(nodename, sizeof(nodename), "gateway\t");
    expected = M2;//martDeviceNodes * mobileUserNodes;
    bytes_received += expected;
  }else if (i<smartDeviceNodes+1){
    snprintf(nodename, sizeof(nodename), "smart device %d ", i);
    expected = mobileUserNodes *M1;
    bytes_received += expected;
  }else{
    snprintf(nodename, sizeof(nodename), "mobile user %d ", i-smartDeviceNodes);
    expected = smartDeviceNodes*M3;
    bytes_received += expected;
  } 

  
  std::cout <<"Number of bytes received at "<<nodename<<"\t : " << totalPacketsThrough <<" / "<< expected << std::endl;
  //uint32_t lost =  DynamicCast<UdpServer> (serverAppContainer.Get (i))->GetLost ();
  //uint32_t window =  DynamicCast<UdpServer> (serverAppContainer.Get (i))->GetPacketWindowSize ();
  //std::cout <<"\t. Packets lost: " << lost <<"( "<<window<<" ) "<< std::endl;
}

  std::cout <<"Total bytes received ("<<mobileUserNodes<<" , "<<smartDeviceNodes<<") : "<< bytes_received << std::endl;
  return 0;
}




