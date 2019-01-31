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

char * stringbuilder( char* prefix,  char* sufix){
  char* buf = (char*)malloc(50); 
  snprintf(buf, 50, "%s%s", prefix, sufix);
  return  buf;
}


ApplicationContainer sendMessage(ApplicationContainer apps, double time, Ptr<Node>source,Ptr<Node>sink, uint32_t packetSize){
    Ipv4Address  remoteAddress = sink->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    /*V4PingHelper ping4(remoteAddress);  // remote address
    //ping4.SetAttribute ("MaxPackets", UintegerValue (1));
    ping4.SetAttribute ("Interval", TimeValue (Seconds (25.)));
    ping4.SetAttribute ("Size", UintegerValue (packetSize));
    ApplicationContainer apps = ping4.Install (source);
    apps.Start (Seconds (3.0));
    apps.Stop (Seconds (15.0));
*/
    //std::cout <<"message  at  "<<time<<std::endl;
  uint16_t port = 9;  // well-known echo port number
  //uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (20.);
  UdpClientHelper client (remoteAddress, port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize+12));
  client.SetAttribute ("StartTime", TimeValue (Seconds (time)));
  //std::cout<<time<<">>";
  apps.Add(client.Install (source));
  return apps;
 }

ApplicationContainer authenticateA(ApplicationContainer appContainer, double time, Ptr<Node> user, Ptr<Node> gateway ){ 

  if (verbose){
    std::cout<<"user : "<< user->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    std::cout<<"    gateway : "<< gateway->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
  }
  uint32_t M1 = 84, M2=64, M3=1, M4 =1;
  appContainer = sendMessage(appContainer, time, user, gateway , M1);
  appContainer = sendMessage(appContainer, time, gateway, user, M2); 
  appContainer = sendMessage(appContainer, time, user, gateway , M3);
  appContainer = sendMessage(appContainer, time, gateway, user, M4); 

  //appContainer = sendMessage(appContainer, time, user, device , M1);
  //appContainer = sendMessage(appContainer, time, device, user, M2); 
  return appContainer;
}

ApplicationContainer authenticateB(ApplicationContainer appContainer, double time, Ptr<Node> user, Ptr<Node> gateway , Ptr<Node> gateway2 ){ 

  if (verbose){
    std::cout<<"user : "<< user->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    std::cout<<"    gateway : "<< gateway->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    std::cout<<"    gateway2 : "<< gateway2->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ()<<std::endl;
  }
  uint32_t M1 = 84, M2=64, M3=1, M4=1;
  appContainer = sendMessage(appContainer, time, user, gateway , M1);
  appContainer = sendMessage(appContainer, time, gateway, gateway2, M1); 
  appContainer = sendMessage(appContainer, time, gateway2, user, M2); 
  appContainer = sendMessage(appContainer, time, user, gateway2 , M3);
  appContainer = sendMessage(appContainer, time, gateway2, user, M4); 

  //appContainer = sendMessage(appContainer, time, user, device , M1);
  //appContainer = sendMessage(appContainer, time, device, user, M2); 
  return appContainer;
}

ApplicationContainer authenticateC(ApplicationContainer appContainer, double time, Ptr<Node> BS, Ptr<Node> CH , Ptr<Node> SN ){ 

  if (verbose){
    std::cout<<"BS : "<< BS->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    std::cout<<"    CH : "<< CH->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
    std::cout<<"    SN : "<< SN->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ()<<std::endl;
  }
  uint32_t M1 = 124, M2=108, M3=44, M4=64;
  appContainer = sendMessage(appContainer, time, BS, CH, M1);
  appContainer = sendMessage(appContainer, time, CH, SN, M2);   
  appContainer = sendMessage(appContainer, time, SN, CH, M3);  
  appContainer = sendMessage(appContainer, time, CH, BS, M4); 

  //appContainer = sendMessage(appContainer, time, user, device , M1);
  //appContainer = sendMessage(appContainer, time, device, user, M2); 
  return appContainer;
}




int 
main (int argc, char *argv[])
{
  
  //
  // First, we declare and initialize a few local variables that control some
  // simulation parameters.
  //
  //uint32_t backboneNodes = 1;//0;
  uint32_t mobileUserNodes = 3;
  uint32_t baseStations = 2;
  uint32_t sensorNodes = 4;
  uint32_t clusterHeads = 2;
  uint32_t stopTime = 2400;
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
  // "--sensorNodes=20"
  //
  CommandLine cmd;
  //cmd.AddValue ("backboneNodes", "number of backbone nodes", backboneNodes);
  cmd.AddValue ("MU", "number of user nodes", mobileUserNodes);  
  cmd.AddValue ("BS", "number of base stations", baseStations);  
  cmd.AddValue ("SN", "number of sensors nodes", sensorNodes);
  cmd.AddValue ("CH", "number of cluster heads", clusterHeads);
  cmd.AddValue ("t", "simulation stop time (seconds)", stopTime);  
  cmd.AddValue ("p", "Enable/disable pcap file generation", enablePcap);
  cmd.AddValue ("a", "Enable/disable xml gneration for netanim-module", enableAnim);
  cmd.AddValue ("o", "Show output end of the simulation", verifyResults);
  cmd.AddValue ("v", "Verbose mode.", verbose);
  cmd.AddValue ("s", "Define the prefix for .pcap anf .xml files. Default: IOT ", saveFilePrefix);




  

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

  std::cout  <<  "MU="<< mobileUserNodes <<", BS="<< baseStations <<", SN="<< sensorNodes << ", CH="<< clusterHeads <<std::endl;

if (verbose)
  {
    LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
    LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
  }



//Since default reference loss is defined for 5 GHz, it needs to be changed when operating at 2.4 GHz
  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceLoss", DoubleValue (40.046));
// Set the maximum wireless range to 5 meters in order to reproduce a hidden nodes scenario, i.e. the distance between hidden stations is larger than 500 meters
  Config::SetDefault ("ns3::RangePropagationLossModel::MaxRange", DoubleValue (500.0));
  
  // creating nodes
  //NodeContainer allNodes;
  NodeContainer wifiUserNodes;
  wifiUserNodes.Create (mobileUserNodes);
  //allNodes.Add (wifiUserNodes);
  NodeContainer wifiNode_SN;
  wifiNode_SN.Create (sensorNodes);

  NodeContainer wifiNode_CH;
  wifiNode_CH.Create (clusterHeads);
  //allNodes.Add (wifiNode_SN);
  NodeContainer wifiGateway ;
  wifiGateway.Create (baseStations);
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

  NetDeviceContainer SmartDevices_CH, SmartDevices_SN;
  SmartDevices_CH = wifi.Install (phy, mac, wifiNode_CH);
  SmartDevices_SN = wifi.Install (phy, mac, wifiNode_SN);
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

  /*MobilityModel mobilityModel =   MobilityModel ();
  mobilityModel.SetAttribute ("Bounds", RectangleValue (Rectangle (-150, 150, -150, 150)));
  mobilityModel.SetAttribute ("Pause", StringValue ("ns3::ConstantRandomVariable[Constant=0.4]"));

  for (uint32_t j = 0; j < wifiUserNodes.GetN (); ++j){
    if(j==0)
      mobilityModel.SetAttribute ("Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
    else if(j==1)
      mobilityModel.SetAttribute ("Speed", StringValue ("ns3::ConstantRandomVariable[Constant=2]"));
    else
      mobilityModel.SetAttribute ("Speed", StringValue ("ns3::ConstantRandomVariable[Constant=15]"));
    mobility.Install (wifiUserNodes.Get(j));
    //std::cout<<RandomMobilityModel.GetAttribute("Speed", StringValue ("ns3::ConstantRandomVariable[Constant=15]"))<<std::endl;
  }

*/

  double BS_pos[baseStations][2],  CH_pos[wifiNode_CH.GetN ()][2];
  uint32_t nearestBS[wifiNode_CH.GetN ()], nearestCH[ wifiNode_SN.GetN ()]; 

   Ptr<ListPositionAllocator> subnetAlloc =   CreateObject<ListPositionAllocator> ();
  subnetAlloc->Add (Vector (0.0, 0.0, 0.0));   //for gateway
  BS_pos[0][0]=0.0;
  BS_pos[0][1]=0.0;
  //std::cout <<"BS_0  = [ "<<BS_pos[0][1]<<","<< BS_pos[0][1]<<"]"<<std::endl;

  for (uint32_t j = 0; j < baseStations-1; ++j){
    double  theta = (j)*360/(baseStations-1);
    uint32_t  r = 100;
    BS_pos[j+1][0]=sin(theta)*r;
    BS_pos[j+1][1]=cos(theta)*r;
    subnetAlloc->Add (Vector (sin(theta)*r, cos(theta)*r, 0.0));
    //std::cout <<"BS_"<<j+1<<"  = [ "<<round(BS_pos[j+1][0])<<","<< round(BS_pos[j+1][1])<<"]"<<std::endl;
  }

  //std::cout <<"___________________"<<std::endl;

 uint32_t  rBS;

  for (uint32_t j = 0; j < wifiNode_CH.GetN (); ++j){
    double  theta = (j)*360/wifiNode_CH.GetN();
    uint32_t  r =((double)rand() / (RAND_MAX))*80 +20;
    rBS = j % baseStations;
    CH_pos[j][0]=   BS_pos[rBS][0] + sin(theta)*r;
    CH_pos[j][1]=   BS_pos[rBS][1] + cos(theta)*r;
    //std::cout <<"CH    = [ "<<round(CH_pos[j][0])<<","<< round(CH_pos[j][1])<<"]"<<std::endl;
    subnetAlloc->Add (Vector (CH_pos[j][0], CH_pos[j][1], 0.0));
    nearestBS[j] = 0;
    double minval = 1000;
    for(uint32_t k = 0; k < baseStations; ++k){
      double d = sqrt( pow(BS_pos[k][0]-CH_pos[j][0] ,2) + pow(BS_pos[k][1]-CH_pos[j][1],2)  );
      if (d < minval){
        nearestBS[j] = k;
        minval = d;
      }
    }
    //std::cout<<"For CH :"<<j <<"   ,    nearestBS is  : "<<nearestBS[j] <<std::endl;
    //std::cout <<"[ "<<sin(theta)*r<<","<< cos(theta)*r<<","<< theta<<"]"<<"r = "<<r<<std::endl;
  }
 //std::cout <<"___________________"<<std::endl;
 double x, y;
  for (uint32_t j = 0; j < wifiNode_SN.GetN (); ++j){
    double  theta = (j)*360/wifiNode_SN.GetN();
    uint32_t  r =((double)rand() / (RAND_MAX))*30 ;
    rBS =  j % clusterHeads;    
    x =   CH_pos[rBS][0] + (sin(theta)*r);
    y =   CH_pos[rBS][1] + (cos(theta)*r);
    subnetAlloc->Add (Vector (x , y, 0.0));
    //std::cout <<"SN    = ["<<round( x )<<","<< round( y )<<"]"<<std::endl;

    nearestCH[j] = 0;
    double minval = 1000;
    for(uint32_t k = 0; k < baseStations; ++k){
      double d = sqrt( pow(CH_pos[k][0]-sin(theta)*r ,2) + pow(CH_pos[k][1]-cos(theta)*r ,2)  );
      if (d < minval){
        nearestCH[j] = k;
        minval = d;
      }
    }


    //std::cout <<"For SN :"<<j <<"   ,    nearestCH is  : "<<nearestCH[j] <<std::endl;
   
  }
  /*std::cout <<"TTnearestBS is  : "<<nearestBS[0] <<std::endl;
  std::cout <<"TTTnearestBS is  : "<<BS_pos[0][0] <<std::endl;
  std::cout <<"TTnearestCH is  : "<<nearestCH[0] <<std::endl;
  std::cout <<"TTTnearestCH is  : "<<CH_pos[0][0] <<std::endl;
*/

  mobility.SetPositionAllocator (subnetAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiGateway);
  mobility.Install (wifiNode_CH);  
  mobility.Install (wifiNode_SN);




 




  // Installing internet stack

  InternetStackHelper stack;
  OlsrHelper olsr;
  stack.SetRoutingHelper (olsr); // has effect on the next Install ()??
  stack.Install (wifiUserNodes);
  stack.Install (wifiNode_CH);  
  stack.Install (wifiNode_SN);
  stack.Install (wifiGateway);
  // Install Ipv4 addresses

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterface;
  apInterface = address.Assign (apDevices);
  //Ipv4InterfaceContainer userInterfaces;
  apInterface = address.Assign (UserDevices);
  //Ipv4InterfaceContainer deviceInterfaces;
  apInterface = address.Assign (SmartDevices_CH);
  apInterface = address.Assign (SmartDevices_SN);



  // crating applications
  ApplicationContainer serverAppContainer, clientAppContainer;  
  uint16_t port = 9;  // well-known echo port number
  UdpServerHelper server(port); 

  for (uint32_t j = 0; j < wifiNode_CH.GetN (); ++j){
    serverAppContainer.Add(server.Install (wifiNode_CH.Get (j)));
  }
  for (uint32_t j = 0; j < wifiNode_SN.GetN (); ++j){
    serverAppContainer.Add(server.Install (wifiNode_SN.Get (j)));
  }
  for (uint32_t j = 0; j < wifiGateway.GetN (); ++j){
    serverAppContainer.Add(server.Install (wifiGateway.Get (j)));
  }

    
double time = 1;
//uint32_t skip = sensorNodes/mobileUserNodes;


for (uint32_t i = 0; i < wifiUserNodes.GetN (); ++i){
  Ptr<Node> user = wifiUserNodes.Get (i);

  for (uint32_t j = i; j < sensorNodes; j+=mobileUserNodes
    ){
    //std::cout <<i<<","<<j<<std::endl;
    //std::cout <<"For SN :"<<j <<"   ,    nearestCH is  : "<<nearestCH[j] <<std::endl;
    //std::cout <<"SN : "<<j <<", CH : "<<nearestCH[j] <<", BS :"<< nearestBS[ nearestCH[j] ] <<std::endl;
    Ptr<Node> CH = wifiNode_CH.Get (nearestCH[j] );
    Ptr<Node> BS = wifiGateway.Get (nearestBS[ nearestCH[j] ] );
    Ptr<Node> SN = wifiNode_SN.Get (j);
    
    if(nearestBS[ nearestCH[j] ] == 0){
      clientAppContainer = authenticateA(clientAppContainer, time , user, BS); 
    }else{
      clientAppContainer = authenticateB(clientAppContainer, time , user, wifiGateway.Get (0), BS); 
    }
    clientAppContainer = authenticateC(clientAppContainer, time , BS, CH, SN); 
    //time = time +.2;   
    //std::cout <<time<<std::endl;
  }
  serverAppContainer.Add(server.Install (user));
  //time = time +.1;
  //std::cout <<"user "<<i<<std::endl;
}

//std::cout <<"clear"<<std::endl;



  serverAppContainer.Start (Seconds (0.0));
  serverAppContainer.Stop (Seconds (stopTime+1));
  


  //clientAppContainer.Start (Seconds (1.0));   //started induvugualy
  clientAppContainer.Stop (Seconds (stopTime+1));


std::cout <<"Setup Compete."<<std::endl;
  
 

if (verbose){
  std::cout <<"servers stops at  "<<stopTime+1<<std::endl;
  std::cout <<"final transmission  scheduled at  "<<(time-.33)<<std::endl;

  std::cout << "server apps installed till now :"<<serverAppContainer.GetN ()<< std::endl;
  std::cout << "client apps installed till now :"<<clientAppContainer.GetN ()<< std::endl;
}

  snprintf(saveFilePrefix, 50, "IOT_%dx%dx%dx%d_", mobileUserNodes, baseStations, clusterHeads, sensorNodes);

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
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_devices"), SmartDevices_CH, 0);
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_devices"), SmartDevices_SN, 0);
    phy.EnablePcap (stringbuilder(saveFilePrefix,(char*)"_gateway"), apDevices, 0);
}


if(enableAnim) {
  AnimationInterface anim (stringbuilder(saveFilePrefix,(char*)"-animation.xml")); // Mandatory
  for (uint32_t i = 0; i < wifiUserNodes.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiUserNodes.Get (i), "MU"); // Optional
      anim.UpdateNodeColor (wifiUserNodes.Get (i), 255, 0, 0); // Optional
    }
  for (uint32_t i = 0; i < wifiNode_CH.GetN (); ++i)
    {
      anim.UpdateNodeDescription (wifiNode_CH.Get (i), "SD"); // Optional
      anim.UpdateNodeColor (wifiNode_CH.Get (i), 255, 255, 0); // Optional
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




  uint32_t bytes_received = 0, totalPacketsThrough;

for (uint32_t i = 0; i < serverAppContainer.GetN (); ++i){
  totalPacketsThrough = DynamicCast<UdpServer> (serverAppContainer.Get (i))->GetReceived ();;
  bytes_received += totalPacketsThrough ;
}

  std::cout <<"Total packets received ("<<    "MU="<< mobileUserNodes <<", BS="<< baseStations <<", SN="<< sensorNodes << ", CH="<< clusterHeads              <<") : "<< bytes_received << std::endl;
  return 0;
}




