/* GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Soumya Banerjee (Adapted from example file wireless-animation.cc)
 */

#include <stdlib.h>    
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

using namespace ns3;




char * stringbuilder( char* prefix,  char* sufix){
  char* buf = (char*)malloc(50); 
  snprintf(buf, 50, "%s%s", prefix, sufix);
  return  buf;
}


ApplicationContainer sendMessage(ApplicationContainer apps, double time, Ptr<Node>source,Ptr<Node>sink, uint32_t packetSize){
  Ipv4Address  remoteAddress = sink->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();

  uint16_t port = 9;  // well-known echo port number
  uint32_t maxPacketCount = 1;
  Time interPacketInterval = Seconds (20.);
  UdpClientHelper client (remoteAddress, port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize+12));
  client.SetAttribute ("StartTime", TimeValue (Seconds (time)));
  
  apps.Add(client.Install (source));
  return apps;
 }

ApplicationContainer authenticateA(ApplicationContainer appContainer, double time, Ptr<Node> user, Ptr<Node> gateway ){ 

  uint32_t M1 = 84, M2=64, M3=1, M4 =1;
  appContainer = sendMessage(appContainer, time, user, gateway , M1);
  appContainer = sendMessage(appContainer, time, gateway, user, M2); 
  appContainer = sendMessage(appContainer, time, user, gateway , M3);
  appContainer = sendMessage(appContainer, time, gateway, user, M4); 

  return appContainer;
}

ApplicationContainer authenticateB(ApplicationContainer appContainer, double time, Ptr<Node> user, Ptr<Node> gateway , Ptr<Node> gateway2 ){ 

  uint32_t M1 = 84, M2=64, M3=1, M4=1;
  appContainer = sendMessage(appContainer, time, user, gateway , M1);
  appContainer = sendMessage(appContainer, time, gateway, gateway2, M1); 
  appContainer = sendMessage(appContainer, time, gateway2, user, M2); 
  appContainer = sendMessage(appContainer, time, user, gateway2 , M3);
  appContainer = sendMessage(appContainer, time, gateway2, user, M4); 

  return appContainer;
}

ApplicationContainer authenticateC(ApplicationContainer appContainer, double time, Ptr<Node> BS, Ptr<Node> CH , Ptr<Node> SN ){ 

  uint32_t M1 = 124, M2=108, M3=44, M4=64;
  appContainer = sendMessage(appContainer, time, BS, CH, M1);
  appContainer = sendMessage(appContainer, time, CH, SN, M2);   
  appContainer = sendMessage(appContainer, time, SN, CH, M3);  
  appContainer = sendMessage(appContainer, time, CH, BS, M4); 

  return appContainer;
}




int 
main (int argc, char *argv[])
{
  
  //
  // First, we declare and initialize a few local variables that control some

  // simulation parameters.
  uint32_t mobileUserNodes = 3;
  uint32_t baseStations = 2;
  uint32_t sensorNodes = 4;
  uint32_t clusterHeads = 2;
  uint32_t stopTime = 2400;
  bool skipApplications =0;
  bool verbose = 0; 
  char saveFilePrefix[50] ;
  

 
  CommandLine cmd;
  cmd.AddValue ("MU", "number of user nodes", mobileUserNodes);  
  cmd.AddValue ("BS", "number of base stations", baseStations);  
  cmd.AddValue ("SN", "number of sensors nodes", sensorNodes);
  cmd.AddValue ("CH", "number of cluster heads", clusterHeads);
  cmd.AddValue ("t", "simulation stop time (seconds)", stopTime); 
  cmd.AddValue ("na", "Dont install applications", skipApplications);
  cmd.AddValue ("v", "Verbose mode.", verbose);
  cmd.AddValue ("s", "Define the prefix for .pcap anf .xml files. Default: WSN ", saveFilePrefix);




  

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



  // creating nodes

  NodeContainer wifiUserNodes;
  wifiUserNodes.Create (mobileUserNodes);
  NodeContainer wifiNode_SN;
  wifiNode_SN.Create (sensorNodes);
  NodeContainer wifiNode_CH;
  wifiNode_CH.Create (clusterHeads);
  NodeContainer wifiGateway ;
  wifiGateway.Create (baseStations);

  // creating wireless channel


  Config::SetDefault  ("ns3::RangePropagationLossModel::MaxRange", DoubleValue (500.0));  
    YansWifiChannelHelper channel_BS ;
    channel_BS.SetPropagationDelay( "ns3::ConstantSpeedPropagationDelayModel" );
    channel_BS.AddPropagationLoss ("ns3::RangePropagationLossModel"); 

  YansWifiPhyHelper phy_BS = YansWifiPhyHelper::Default ();
    phy_BS.SetChannel (channel_BS.Create ());

  Config::SetDefault  ("ns3::RangePropagationLossModel::MaxRange", DoubleValue (30.0));  
  YansWifiChannelHelper channel_SN ;//= YansWifiChannelHelper::Default ();
    channel_SN.SetPropagationDelay( "ns3::ConstantSpeedPropagationDelayModel" );
    channel_SN.AddPropagationLoss ("ns3::RangePropagationLossModel"); 

  YansWifiPhyHelper phy_SN = YansWifiPhyHelper::Default ();
    phy_SN.SetChannel (channel_SN.Create ());
  
  Config::SetDefault  ("ns3::RangePropagationLossModel::MaxRange", DoubleValue (100.0));  
  YansWifiChannelHelper channel_CH ;//= YansWifiChannelHelper::Default ();
    channel_CH.SetPropagationDelay( "ns3::ConstantSpeedPropagationDelayModel" );
    channel_CH.AddPropagationLoss ("ns3::RangePropagationLossModel"); 
 
  YansWifiPhyHelper phy_CH = YansWifiPhyHelper::Default ();
    phy_CH.SetChannel (channel_CH.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac_STA, mac_AP;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac_STA.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  mac_AP.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));




  

  NetDeviceContainer  UserDevices = wifi.Install (phy_BS, mac_STA, wifiUserNodes); 
  NetDeviceContainer SmartDevices_SN = wifi.Install (phy_SN, mac_STA, wifiNode_SN);
  NetDeviceContainer SmartDevices_CH( wifi.Install (phy_CH, mac_STA, wifiNode_CH),
                                      wifi.Install (phy_SN, mac_STA, wifiNode_CH)    );  
  NetDeviceContainer apDevices( wifi.Install (phy_BS, mac_AP, wifiGateway),
                                 wifi.Install (phy_CH, mac_AP, wifiGateway)  );




 

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

  

  double BS_pos[baseStations][2],  CH_pos[wifiNode_CH.GetN ()][2];
  uint32_t nearestBS[wifiNode_CH.GetN ()], nearestCH[ wifiNode_SN.GetN ()]; 

  Ptr<ListPositionAllocator> subnetAlloc =   CreateObject<ListPositionAllocator> ();

  subnetAlloc->Add (Vector (0.0, 0.0, 0.0));   //for BS_0
  BS_pos[0][0]=0.0;
  BS_pos[0][1]=0.0;

  for (uint32_t j = 0; j < baseStations-1; ++j){
    double  theta = (j)*360/(baseStations-1);
    uint32_t  r = 200;
    BS_pos[j+1][0]=sin(theta)*r;
    BS_pos[j+1][1]=cos(theta)*r;
    subnetAlloc->Add (Vector (sin(theta)*r, cos(theta)*r, 0.0));
  }

 
 uint32_t  rBS;

  for (uint32_t j = 0; j < wifiNode_CH.GetN (); ++j){
    double  theta = (j)*360/wifiNode_CH.GetN();
    uint32_t  r =((double)rand() / (RAND_MAX))*90 ;
    rBS = j % baseStations;
    CH_pos[j][0]=   BS_pos[rBS][0] + sin(theta)*r;
    CH_pos[j][1]=   BS_pos[rBS][1] + cos(theta)*r;
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
  }

 double x, y;
  for (uint32_t j = 0; j < wifiNode_SN.GetN (); ++j){
    double  theta = (j)*360/wifiNode_SN.GetN();
    uint32_t  r =((double)rand() / (RAND_MAX))*20 ;
    rBS =  j % clusterHeads;    
    x =   CH_pos[rBS][0] + (sin(theta)*r);
    y =   CH_pos[rBS][1] + (cos(theta)*r);
    subnetAlloc->Add (Vector (x , y, 0.0));

    nearestCH[j] = 0;
    double minval = 1000;
    for(uint32_t k = 0; k < baseStations; ++k){
      double d = sqrt( pow(CH_pos[k][0]-sin(theta)*r ,2) + pow(CH_pos[k][1]-cos(theta)*r ,2)  );
      if (d < minval){
        nearestCH[j] = k;
        minval = d;
      }
    }   
  }

  mobility.SetPositionAllocator (subnetAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiGateway);
  mobility.Install (wifiNode_CH);  
  mobility.Install (wifiNode_SN);


  // Installing internet stack

  InternetStackHelper stack;
  OlsrHelper olsr;
  stack.SetRoutingHelper (olsr); 
  stack.Install (wifiUserNodes);
  stack.Install (wifiNode_CH);  
  stack.Install (wifiNode_SN);
  stack.Install (wifiGateway);


  // Install Ipv4 addresses
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterface;
  apInterface = address.Assign (apDevices);
  apInterface = address.Assign (UserDevices);
  apInterface = address.Assign (SmartDevices_CH);
  apInterface = address.Assign (SmartDevices_SN);



  // Crating applications

  ApplicationContainer serverAppContainer, clientAppContainer;

  if (skipApplications){
    std::cout <<"Setup skipping applications."<<std::endl;
  } else {      
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

      
  double time = 1; // to stagger authentication messages (if desired)
  for (uint32_t i = 0; i < wifiUserNodes.GetN (); ++i){
    Ptr<Node> user = wifiUserNodes.Get (i);
    for (uint32_t j = i; j < sensorNodes; j+=mobileUserNodes){

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
    }
    serverAppContainer.Add(server.Install (user));
    //time = time +.1;
  }

  



    serverAppContainer.Start (Seconds (0.0));
    serverAppContainer.Stop (Seconds (stopTime+1));
    


    //clientAppContainer.Start (Seconds (1.0));   //started individually
    clientAppContainer.Stop (Seconds (stopTime+1));

}



  std::cout <<"Setup Compete."<<std::endl;  
 



  snprintf(saveFilePrefix, 50, "WSN_%dx%dx%dx%d_", mobileUserNodes, baseStations, clusterHeads, sensorNodes);






  //Populate routing table
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // setting up simulator
  

  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  Simulator::Stop (Seconds (stopTime+1));
  Simulator::Run ();
  Simulator::Destroy ();

  // Generating output

  flowMonitor->SerializeToXmlFile(stringbuilder(saveFilePrefix,(char*)"_flowMonitor.xml"), false, false);
  if (!skipApplications){
    uint32_t bytes_received = 0, totalPacketsThrough;
    for (uint32_t i = 0; i < serverAppContainer.GetN (); ++i){
      totalPacketsThrough = DynamicCast<UdpServer> (serverAppContainer.Get (i))->GetReceived ();;
      bytes_received += totalPacketsThrough ;
    }
    std::cout <<"Total packets received ("<<    "MU="<< mobileUserNodes <<", BS="<< baseStations <<", SN="<< sensorNodes << ", CH="<< clusterHeads              <<") : "<< bytes_received << std::endl;
  }
  return 0;
}




