#include <Arduino.h>
#include <PanelLeaf.h>

// Definitions and variables
#define RGB_STRIP_PIN 5
#define RGB_STRIP_QTY_LEDS 18

char ADDRESS = NONE;
const char EDGES_CODES[3] = {0x01, 0x02, 0x03};
const int EDGES_PINS[3] = {2, 3, 4};

// Instantiate controllers
SerialBus serialBus(SERIAL_RATE);
EdgesController edges(EDGES_CODES, EDGES_PINS);
RGBStripController rgbStrip(RGB_STRIP_PIN, RGB_STRIP_QTY_LEDS);

// Define the methods
void RegisterEvents();
void NetworkRegisterEventListener(RequestHeader *h);
void SetEdgeLevelEventListener(RequestHeader *h);
void SetColorEventListener(RequestHeader *h);
void FadeColorEventListener(RequestHeader *h);

void setup()
{
  serialBus.begin();
  edges.begin();
  rgbStrip.begin();

  RegisterEvents();
}

void RegisterEvents()
{
  serialBus.clearEvents();
  serialBus.registerEvent(CMD_SET_ADDRESS, CHANNEL_ADDRESS, NetworkRegisterEventListener);
  serialBus.registerEvent(CMD_SET_EDGE_LEVEL, ADDRESS, SetEdgeLevelEventListener);
  serialBus.registerEvent(CMD_SET_COLOR, ADDRESS, SetColorEventListener);
  serialBus.registerEvent(CMD_FADE_COLOR, ADDRESS, FadeColorEventListener);
}

void NetworkRegisterEventListener(RequestHeader *h)
{
  // Get the trigged edge
  char triggedEdge = edges.getTriggedOne();

  // Check if is this tile contains one edge trigged, otherwise it's other tile to be addressed
  if (triggedEdge != NONE)
  {
    // Read payload
    TileAddress t;
    serialBus.readPayload(&t);

    // Change the address
    ADDRESS = t.Address;
    RegisterEvents(); // Update the address in events

    // Create a response with actual address and trigged edge
    TileAddressResponse res = TileAddressResponse{ADDRESS, triggedEdge};

    // Send the message to join in network
    serialBus.sendCommand(CMD_JOIN_ADDRESS, CHANNEL_ADDRESS, &res);
  }
}

void SetEdgeLevelEventListener(RequestHeader *h)
{
  // Read payload
  EdgeLevel edgeLevel;
  serialBus.readPayload(&edgeLevel);

  // Set the level of the edge
  edges.setStatus(edgeLevel.Edge, edgeLevel.Status == 0x01);
}

void SetColorEventListener(RequestHeader *h)
{
  // Read payload
  RGB rgb;
  serialBus.readPayload(&rgb);

  // Change the RGB strip color
  rgbStrip.setColor(rgb);
}

void FadeColorEventListener(RequestHeader *h)
{
  // Read payload
  FadeColor fadeColor;
  serialBus.readPayload(&fadeColor);

  // Change the RGB strip color
  rgbStrip.fadeColor(fadeColor.Color, fadeColor.Time);
}

void loop()
{
  serialBus.tick();
  rgbStrip.tick();
}
