#pragma once

class ChatScreen;

class CustomChatScreen
{
public:
	static void(*sendChatMessage_real)(ChatScreen *);
	static void sendChatMessage(ChatScreen *);

	static void setupHooks();
};
