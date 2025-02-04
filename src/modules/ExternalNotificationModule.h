#pragma once

#include "SinglePortModule.h"
#include "concurrency/OSThread.h"
#include "configuration.h"
#ifndef ARCH_PORTDUINO
#include <NonBlockingRtttl.h>
#else
// Noop class for portduino.
class rtttl 
{
  public:
    explicit rtttl() {}
    static bool isPlaying() { return false; }
    static void play() {}
    static void begin(byte a, const char * b) {};
    static void stop() {}
    static bool done() { return true; }
};
#endif
#include <Arduino.h>
#include <functional>

/*
 * Radio interface for ExternalNotificationModule
 *
 */
class ExternalNotificationModule : public SinglePortModule, private concurrency::OSThread
{
  uint32_t output = 0;

  public:
    ExternalNotificationModule();

    uint32_t nagCycleCutoff = UINT32_MAX;

    void setExternalOn(uint8_t index = 0);
    void setExternalOff(uint8_t index = 0);
    bool getExternal(uint8_t index = 0);

    void stopNow();

    void handleGetRingtone(const MeshPacket &req, AdminMessage *response);
    void handleSetRingtone(const char *from_msg);

  protected:
    /** Called to handle a particular incoming message
    @return ProcessMessage::STOP if you've guaranteed you've handled this message and no other handlers should be considered for it
    */
    virtual ProcessMessage handleReceived(const MeshPacket &mp) override;

    virtual int32_t runOnce() override;

    bool isNagging = false;

    virtual AdminMessageHandleResult handleAdminMessageForModule(const MeshPacket &mp, AdminMessage *request, AdminMessage *response) override;
};

extern ExternalNotificationModule *externalNotificationModule;
