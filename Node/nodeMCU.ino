#include "painlessMesh.h"
extern "C" {
  #include <user_interface.h>
}

#define DATA_LENGTH           112

#define TYPE_MANAGEMENT       0x00
#define TYPE_CONTROL          0x01
#define TYPE_DATA             0x02
#define SUBTYPE_PROBE_REQUEST 0x04
#define CHANNEL_HOP_INTERVAL_MS   1000


#define DISABLE 0
#define ENABLE  1


#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

String addMac = "0";
String dadosDevice = "";
String dados = "";
String dadosRssi = "";
String dadosAddr = "";
String dadosCh = "";
String dadosSsid = "";

uint32_t gateway = 2785176934;

painlessMesh  mesh;

struct RxControl {
 signed rssi:8; // signal intensity of packet
 unsigned rate:4;
 unsigned is_group:1;
 unsigned:1;
 unsigned sig_mode:2; // 0:is 11n packet; 1:is not 11n packet;
 unsigned legacy_length:12; // if not 11n packet, shows length of packet.
 unsigned damatch0:1;
 unsigned damatch1:1;
 unsigned bssidmatch0:1;
 unsigned bssidmatch1:1;
 unsigned MCS:7; // if is 11n packet, shows the modulation and code used (range from 0 to 76)
 unsigned CWB:1; // if is 11n packet, shows if is HT40 packet or not
 unsigned HT_length:16;// if is 11n packet, shows length of packet.
 unsigned Smoothing:1;
 unsigned Not_Sounding:1;
 unsigned:1;
 unsigned Aggregation:1;
 unsigned STBC:2;
 unsigned FEC_CODING:1; // if is 11n packet, shows if is LDPC packet or not.
 unsigned SGI:1;
 unsigned rxend_state:8;
 unsigned ampdu_cnt:8;
 unsigned channel:4; //which channel this packet in.
 unsigned:12;
};

struct SnifferPacket{
    struct RxControl rx_ctrl;
    uint8_t data[DATA_LENGTH];
    uint16_t cnt;
    uint16_t len;
};

static void showMetadata(struct SnifferPacket *snifferPacket) {


  unsigned int frameControl = ((unsigned int)snifferPacket->data[1] << 8) + snifferPacket->data[0];

  uint8_t version      = (frameControl & 0b0000000000000011) >> 0;
  uint8_t frameType    = (frameControl & 0b0000000000001100) >> 2;
  uint8_t frameSubType = (frameControl & 0b0000000011110000) >> 4;
  uint8_t toDS         = (frameControl & 0b0000000100000000) >> 8;
  uint8_t fromDS       = (frameControl & 0b0000001000000000) >> 9;

  // Only look for probe request packets
  if (frameType != TYPE_MANAGEMENT ||
      frameSubType != SUBTYPE_PROBE_REQUEST)
        return;

  Serial.print("RSSI: ");
  Serial.print(snifferPacket->rx_ctrl.rssi, DEC);
  dadosRssi = snifferPacket->rx_ctrl.rssi;

  Serial.print(" Ch: ");
  Serial.print(wifi_get_channel());
  dadosCh = wifi_get_channel();

  char addr[] = "00:00:00:00:00:00";
  getMAC(addr, snifferPacket->data, 10);
  Serial.print(" Peer MAC: ");
  Serial.print(addr);
  dadosAddr = addr;

  uint8_t SSID_length = snifferPacket->data[25];
  Serial.print(" SSID: ");
  dadosSsid = printDataSpan(26, SSID_length, snifferPacket->data);

  Serial.println();

  uint32_t nodeId = mesh.getNodeId();

  dadosDevice += nodeId + "," + dadosRssi + "," + dadosCh + "," + dadosAddr + "," + dadosSsid + "\n";

}

/**
 * Callback for promiscuous mode
 */
static void ICACHE_FLASH_ATTR sniffer_callback(uint8_t *buffer, uint16_t length) {
  struct SnifferPacket *snifferPacket = (struct SnifferPacket*) buffer;
  showMetadata(snifferPacket);
}

static String printDataSpan(uint16_t start, uint16_t size, uint8_t* data) {
  String strSsid;
  char n;
  for(uint16_t i = start; i < DATA_LENGTH && i < start+size; i++) {
//    Serial.write(data[i]);
    n = data[i];
    strSsid.concat(n);
  }
  return strSsid;
}

static void getMAC(char *addr, uint8_t* data, uint16_t offset) {
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", data[offset+0], data[offset+1], data[offset+2], data[offset+3], data[offset+4], data[offset+5]);
}


static os_timer_t channelHop_timer;

/**
 * Callback for channel hoping
 */
void channelHop()
{
  // hoping channels 1-14
  uint8 new_channel = wifi_get_channel() + 1;
  if (new_channel > 14)
    new_channel = 1;
  wifi_set_channel(new_channel);
}

bool modo = true;
Task taskChaveamento ( TASK_SECOND * 15 , TASK_FOREVER, &chaveamento);
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void chaveamento(){
  if (modo == true){
    Serial.println("Modo Mesh");
    wifi_set_opmode(STATIONAP_MODE);
    os_timer_disarm(&channelHop_timer);
    wifi_promiscuous_enable(DISABLE);
    wifi_set_channel(1);

    meshStart();
    modo = false;
    taskChaveamento.delay(15000);
    } else {
      Serial.println("Modo Sniffer");
      mesh.stop();
      snifferStart();
      modo = true;
      }
  }

void snifferStart() {
  // set the WiFi chip to "promiscuous" mode aka monitor mode

  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(1);
  wifi_promiscuous_enable(DISABLE);

  wifi_set_promiscuous_rx_cb(sniffer_callback);

  wifi_promiscuous_enable(ENABLE);

  // setup the channel hoping callback timer
  os_timer_disarm(&channelHop_timer);
  os_timer_setfn(&channelHop_timer, (os_timer_func_t *) channelHop, NULL);
  os_timer_arm(&channelHop_timer, CHANNEL_HOP_INTERVAL_MS, 1);

}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void meshStart() {

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  taskSendMessage.enable() ;
}

void sendMessage() {
//  String msg = "Hello from node ";
//  msg += mesh.getNodeId();
//  mesh.sendBroadcast( msg + dadosDevice);
    bool enviado = mesh.sendSingle(gateway, dadosDevice);
    if ( enviado == true){
          dadosDevice = "";
      }
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));

}

void setup() {
  Serial.begin(115200);

  mesh.scheduler.addTask( taskSendMessage );
  mesh.scheduler.addTask( taskChaveamento );

  taskChaveamento.enable();
  }

void loop() {
  mesh.update();
  }