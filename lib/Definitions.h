#ifndef Definitions_h
#define Definitions_h

#define NONE 0x00

#define SERIAL_RATE 115200

#define CHANNEL_ADDRESS 0x00
#define CHANNEL_BRODCAST 0xFF

#define CMD_NULL 0x00
#define CMD_SET_ADDRESS 0x01
#define CMD_JOIN_ADDRESS 0x02
#define CMD_SET_EDGE_LEVEL 0x03
#define CMD_SET_COLOR 0x04
#define CMD_FADE_COLOR 0x05

struct RequestHeader
{
  char Command;
  char Address;
};

struct Event
{
  char Command;
  char *Address;
  void (*Callback)(RequestHeader *);
};

struct TileAddress
{
  char Address;
};

struct TileAddressResponse
{
  char AddressSource;
  char EdgeTrigged;
};

struct EdgeLevel
{
  char Edge;
  char Status;
};

struct RGB
{
  unsigned short R;
  unsigned short G;
  unsigned short B;
};

struct FadeColor
{
  RGB Color;
  unsigned int Time;
};

#endif
