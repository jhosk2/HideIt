HideIt - Lock your data & files on tizen
===================================

HideIt은 타이젠 네이티브 기반의 어플리케이션으로서, 기기 내의 파일을 안전하게 숨기고 다른 사람이 읽지 못하도록 도와줍니다.

## 기능

- [Documented source](http://www.javadoc.io/doc/com.github.theholywaffle/lolchatapi)
- 기능에 충실한 일반 계산기 및 공학용 계산기
- 파일 브라우징
- 파일 암호화 및 복호화

## 시작하기

### 프로젝트 다운로드

- **Github에서 Clone받기**: 

   <a href="https://github.com/jhosk2/HideIt.git" target="_blank">최신 git</a> 를 Clone합니다.

### Usage

1. Create a [LolChat](src/main/java/com/github/theholywaffle/lolchatapi/LolChat.java) object with the correct [ChatServer](src/main/java/com/github/theholywaffle/lolchatapi/ChatServer.java) of your region.
2. Add any listeners.
3. Login.
4. ...

### Example

```java
final LolChat api = new LolChat(ChatServer.EUW, FriendRequestPolicy.ACCEPT_ALL, new RiotApiKey("RIOT-API-KEY", RateLimit.DEFAULT));
if (api.login("myusername", "mypassword")) {

	// Example 1: Send Chat Message to all your friends
	for (Friend f : api.getFriends()) {
		f.sendMessage("Hello " + f.getName());
	}

	// Example 2: Send Chat Message to all your friends and wait for an
	// response
	for (Friend f : api.getFriends()) {
		f.sendMessage("Hello " + f.getName(), new ChatListener() {

			@Override
			public void onMessage(Friend friend, String message) {
				System.out.println("Friend " + friend.getName()
						+ " responded to my Hello World!");
			}
		});
	}

	// Example3: Send Chat Message to an specific friend
	Friend f = api.getFriendByName("Dyrus");
	if (f != null && f.isOnline()) {
		f.sendMessage("Hi, I'm your biggest fan!");
	}
}
```

[more examples](example)

### Javadocs

[here](http://www.javadoc.io/doc/com.github.theholywaffle/lolchatapi)

## Questions or bugs?

Please let me know them [here](../../issues). I'll help you out as soon as I can.

___
*밚eague of Legends XMPP Chat Library isn't endorsed by Riot Games and doesn't reflect the views or opinions of Riot Games or anyone officially involved in producing or managing League of Legends. League of Legends and Riot Games are trademarks or registered trademarks of Riot Games, Inc. League of Legends ?Riot Games, Inc.?
