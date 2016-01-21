#pragma once

#include "SMPlayer.h"

class LocalPlayer;

class SMLocalPlayer : public SMPlayer
{
public:
	SMLocalPlayer(Server *server, LocalPlayer *entity);

	void sendRawMessage(const std::string &message);

	void sendTranslation(const std::string &message, const std::vector<std::string> &params);
	void sendPopup(const std::string &message, const std::string &subtitle = "");
	void sendTip(const std::string &message);

	bool isLocalPlayer() const;

	bool isOp() const;
	void setOp(bool value);

	LocalPlayer *getHandle() const;
};
