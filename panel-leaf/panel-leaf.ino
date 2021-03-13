#include "Network.h"
#include "Definitions.h"
#include "EdgesController.h"
#include "RGBStripController.h"

#define RGB_STRIP_PIN 5
#define RGB_STRIP_QTY_LEDS 18

char ADDRESS = NULL;
const char EDGES_CODES[3] = {0x01, 0x02, 0x03};
const int EDGES_PINS[3] = {2, 3, 4};

EdgesController Edges(EDGES_CODES, EDGES_PINS);
RGBStripController RGBStrip(RGB_STRIP_PIN, RGB_STRIP_QTY_LEDS);

void setup() {
  TransportLayer::begin(SERIAL_RATE);
  Edges.begin();
  RGBStrip.begin();
}

void NetworkRegisterEventListener(RequestHeader* h) {
  if(h->Command == CMD_SET_ADDRESS && h->Address == CHANNEL_ADDRESS && ADDRESS == NULL) {
    // Get the trigged edge
    char triggedEdge = Edges.getTriggedOne();

    // Check if is this tile contains one edge trigged, otherwise it's other tile to be addressed
    if(triggedEdge != NULL) {
      // Read payload
      TileAddress t;
      TransportLayer::readPayload(&t);
  
      ADDRESS = t.Address;
      
      // Create a response with actual address and trigged edge
      TileAddressResponse res = TileAddressResponse{ADDRESS, triggedEdge};
  
      // Send the message to join in network
      TransportLayer::sendCommand(CMD_JOIN_ADDRESS, CHANNEL_ADDRESS, &res);
    }
  }
}

void SetEdgeLevelEventListener(RequestHeader* h) {
  if(h->Command == CMD_SET_EDGE_LEVEL && (h->Address == ADDRESS || h->Address == CHANNEL_BRODCAST)) {
    // Read payload
    EdgeLevel edgeLevel;
    TransportLayer::readPayload(&edgeLevel);

    // Set the level of the edge
    Edges.setStatus(edgeLevel.Edge, edgeLevel.Status);
  }
}

void SetColorEventListener(RequestHeader* h) {
  if(h->Command == CMD_SET_COLOR && (h->Address == ADDRESS || h->Address == CHANNEL_BRODCAST)) {
    // Read payload
    RGB rgb;
    TransportLayer::readPayload(&rgb);

    // Change the RGB strip color
    RGBStrip.setColor(&rgb);
  }
}

void FadeColorEventListener(RequestHeader* h) {
  if(h->Command == CMD_FADE_COLOR && (h->Address == ADDRESS || h->Address == CHANNEL_BRODCAST)) {
    // Read payload
    FadeColor fadeColor;
    TransportLayer::readPayload(&fadeColor);

    // Change the RGB strip color
    RGBStrip.fadeColor(&fadeColor.Color, fadeColor.Time);
  }
}

void CaptureEvents() {
  while(!(Serial.available() > 2));

  // Get the request header
  RequestHeader h;
  TransportLayer::readHeader(&h);

  // Call the events listeners
  NetworkRegisterEventListener(&h);
  SetEdgeLevelEventListener(&h);
  SetColorEventListener(&h);
  FadeColorEventListener(&h);
}

void loop() {
  CaptureEvents();
}
