/** Network Simulator 3.29 LTE Test Program
 * @Author: Ayush Mukherjee
 * @Date: 13 December 2018
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-helper.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>

using namespace ns3;

int main () {
    Time simTime = MilliSeconds (1500);

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults ();

    Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

    lteHelper->EnableLogComponents ();

    NodeContainer enbNodes;
    NodeContainer ueNodes;
    enbNodes.Create (1);
    ueNodes.Create (1);

    // Install Mobility Model
    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (enbNodes);
    BuildingsHelper::Install (enbNodes);
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (ueNodes);
    BuildingsHelper::Install (ueNodes);

    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;
    
    // lteHelper->SetSchedulerType ("ns3:RrFfMacScheduler");

    enbDevs = lteHelper->InstallEnbDevice (enbNodes);
    ueDevs = lteHelper->InstallUeDevice (ueNodes);

    // Attach UE to eNB
    lteHelper->Attach (ueDevs, enbDevs.Get (0));

    // Activate a data radio bearer
    enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
    EpsBearer bearer (q);
    lteHelper->ActivateDataRadioBearer (ueDevs, bearer);
    lteHelper->EnableTraces ();

    Simulator::Stop (simTime);
    Simulator::Run ();

    Simulator::Destroy();
    return 0;
}

