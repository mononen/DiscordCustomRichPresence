/*
This is a simple example in C of using the rich presence API asyncronously.
*/

#define _CRT_SECURE_NO_WARNINGS /* thanks Microsoft */
#define DELIM "="

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "discord-rpc.h"

static const char* APPLICATION_ID = "387412856724783117";
static int FrustrationLevel = 0;
static int64_t StartTime;
char discordState[256] = "Probably doing things?";
char discordDetails[256] = "Staying Frosty";
char discordLargeImageKey[64] = "stay_frosty";
char discordSmallImageKey[64] = "hypesquadbadge";
char discordSmallImageText[512] = "part of the HypeSquad";
char discordLargeImageText[256] = "Is he actually doing anything tho?";
int timer;
FILE *config;

static int prompt(char* line, size_t size)
{
	int res;
	char* nl;
	printf("\n> ");
	fflush(stdout);
	res = fgets(line, (int)size, stdin) ? 1 : 0;
	line[size - 1] = 0;
	nl = strchr(line, '\n');
	if (nl) {
		*nl = 0;
	}
	return res;
}

static void updateDiscordPresence()
{
	//some of the old code
	//char buffer[256];
	//sprintf(buffer, "Frustration level: %d", FrustrationLevel);    
	//discordPresence.endTimestamp = time(0) + 5 * 60;
	//discordPresence.smallImageKey = "ptb-small";
	//discordPresence.partyId = "party1234";
	//discordPresence.partySize = 1;
	//discordPresence.partyMax = 6;
	//discordPresence.joinSecret = "join";
	//discordPresence.spectateSecret = "look";


	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = discordState;
	discordPresence.details = discordDetails;
	discordPresence.startTimestamp = StartTime;
	//discordPresence.largeImageText = discordLargeImageText;
	discordPresence.largeImageKey = discordLargeImageKey;
	if (discordLargeImageText != NULL)
	{
		discordPresence.largeImageText = discordLargeImageText;
	}
	if (discordSmallImageText != NULL)
	{
		discordPresence.smallImageText = discordSmallImageText;
	}
	if (discordSmallImageKey != NULL)
	{
		discordPresence.smallImageKey = discordSmallImageKey;
	}
	discordPresence.matchSecret = "xyzzy";
	discordPresence.instance = 0;
	Discord_UpdatePresence(&discordPresence);
}

static void handleDiscordReady()
{
	printf("\nDiscord: ready\n");
}

static void handleDiscordDisconnected(int errcode, const char* message)
{
	printf("\nDiscord: disconnected (%d: %s)\n", errcode, message);
}

static void handleDiscordError(int errcode, const char* message)
{
	printf("\nDiscord: error (%d: %s)\n", errcode, message);
}

static void handleDiscordJoin(const char* secret)
{
	printf("\nDiscord: join (%s)\n", secret);
}

static void handleDiscordSpectate(const char* secret)
{
	printf("\nDiscord: spectate (%s)\n", secret);
}

static void handleDiscordJoinRequest(const DiscordJoinRequest* request)
{
	int response = -1;
	char yn[4];
	printf("\nDiscord: join request from %s - %s - %s\n",
		request->username,
		request->avatar,
		request->userId);
	do {
		printf("Accept? (y/n)");
		if (!prompt(yn, sizeof(yn))) {
			break;
		}

		if (!yn[0]) {
			continue;
		}

		if (yn[0] == 'y') {
			response = DISCORD_REPLY_YES;
			break;
		}

		if (yn[0] == 'n') {
			response = DISCORD_REPLY_NO;
			break;
		}
	} while (1);
	if (response != -1) {
		Discord_Respond(request->userId, response);
	}
}

static void discordInit()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	handlers.ready = handleDiscordReady;
	handlers.disconnected = handleDiscordDisconnected;
	handlers.errored = handleDiscordError;
	handlers.joinGame = handleDiscordJoin;
	handlers.spectateGame = handleDiscordSpectate;
	handlers.joinRequest = handleDiscordJoinRequest;
	Discord_Initialize(APPLICATION_ID, &handlers, 1, NULL);
}

static void gameLoop()
{
	char line[512];
	char* space;

	//sets it to like 17 hours
	StartTime = time(0) - 10000000;

	printf("You are standing in an open field west of a white house.\n");
	printf("q = quit, y = reinit");
	while (prompt(line, sizeof(line))) {
		if (line[0]) {
			if (line[0] == 'q') {
				break;
			}

			if (line[0] == 't') {
				printf("Shutting off Discord.\n");
				Discord_Shutdown();
				continue;
			}

			if (line[0] == 'y') {
				printf("Reinit Discord.\n");
				discordInit();
				continue;
			}
			if (line[0] == 'p')
			{
				strcpy(discordSmallImageKey, "hypesquadbadge");
				discordInit();
			}
			if (line[0] == 'c')
			{
				printf("change what? \n");
				char command[512];
				scanf_s("%s", &command);
				if (strcmp(command, "largekey") == 0)
				{
					char specific[64];
					scanf_s("%s", &specific);
					strcpy(discordLargeImageKey, specific);
				}
				if (strcmp(command, "largetext") == 0)
				{
					char specific[64];
					scanf_s("%s", &specific);
					strcpy(discordLargeImageText, specific);
				}
				if (strcmp(command, "smallkey") == 0)
				{
					char specific[64];
					scanf_s("%s", &specific);
					strcpy(discordSmallImageKey, specific);
				}
				if (strcmp(command, "smalltext") == 0)
				{
					char specific[64];
					scanf_s("%s", &specific);
					strcpy(discordSmallImageText, specific);
				}
				if (strcmp(command, "details") == 0)
				{
					char specific[64];
					scanf_s("%s", &specific);
					strcpy(discordDetails, specific);
				}
				if (strcmp(command, "state") == 0)
				{
					char specific[64];
					scanf_s("%s", &specific);
					strcpy(discordState, specific);
				}
				printf("Reinit Discord. \n");
				discordInit();
				continue;
			}

			if (line[0] == 'r')
			{
				reboot();
			}
			if (time(NULL) & 1) {
				printf("I don't understand that.\n");
			}

			else {
				space = strchr(line, ' ');
				if (space) {
					*space = 0;
				}
				printf("I don't know the word \"%s\".\n", line);
			}

			++FrustrationLevel;

			updateDiscordPresence();
		}

#ifdef DISCORD_DISABLE_IO_THREAD
		Discord_UpdateConnection();
#endif
		Discord_RunCallbacks();
	}
}

int countlines(char *filename)
{
	FILE *fp = fopen(filename, "r");
	int ch = 0;
	int lines = 0;

	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n')
		{
			lines++;
		}
	}

	return lines;
}

int reboot()
{
	writeConfig();
	readConfig();
	system("send-presence.exe");
	exit();
	return 0;
}

/*
void copyConfig()
{
config = fopen("config.txt", "r");

if (config != NULL)
{
char line[2048];
int i = 0;

while ((fgets(line, sizeof(line), config) != NULL) && i < 7)
{
char *cfline;
cfline = strstr((char *)line, DELIM);
cfline = cfline + strlen(DELIM);

if (i == 0) {
memcpy(discordLargeImageKey, cfline, strlen(cfline));
//printf("%s",configstruct.imgserver);
}
else if (i == 1) {
memcpy(discordSmallImageKey, cfline, strlen(cfline));
//printf("%s",configstruct.ccserver);
}
else if (i == 2) {
memcpy(discordLargeImageText, cfline, strlen(cfline));
//printf("%s",configstruct.port);
}
else if (i == 3) {
memcpy(discordSmallImageText, cfline, strlen(cfline));
//printf("%s",configstruct.imagename);
}
else if (i == 4) {
memcpy(discordDetails, cfline, strlen(cfline));
//printf("%s",configstruct.getcmd);
}
else if (i == 5)
{
memcpy(discordState, cfline, strlen(cfline));
}

i++;
} // End while
fclose(config);
} // End if
}
*/

int readConfig()
{
	config = fopen("config.txt", "r");
	char buf[1000];
	int i = 0;
	if (!config) { return 1; }
	while (fgets(buf, 1000, config) != NULL)
	{
		if (i == 0)
		{
			strcpy(discordLargeImageKey, buf);
		}
		if (i == 1)
		{
			strcpy(discordSmallImageKey, buf);
		}
		if (i == 2)
		{
			strcpy(discordLargeImageText, buf);
		}
		if (i == 3)
		{
			strcpy(discordSmallImageText, buf);
		}
		if (i == 4)
		{
			strcpy(discordDetails, buf);
		}
		if (i == 5)
		{
			strcpy(discordState, buf);
		}
	}
	fclose(config);
	return 0;
}

int writeConfig()
{
	clearConfig();
	config = fopen("config.txt", "a");
	fprintf(config, "%s", discordLargeImageKey);
	fprintf(config, "%s", discordSmallImageKey);
	fprintf(config, "%s", discordLargeImageText);
	fprintf(config, "%s", discordSmallImageText);
	fprintf(config, "%s", discordDetails);
	fprintf(config, "%s", discordState);
	fclose(config);
	return 0;
}

int clearConfig()
{
	config = fopen("config.tex", "w");
	fclose(config);
	return 0;
}

int main(int argc, char* argv[])
{
	//config = fopen("config.txt", "r+");
	//for each line in the document, take the header and set it equal to the preceeding text
	readConfig();

	discordInit();

	gameLoop();

	Discord_Shutdown();
	return 0;
}
