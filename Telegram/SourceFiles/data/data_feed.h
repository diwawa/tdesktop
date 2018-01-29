/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "dialogs/dialogs_entry.h"
#include "data/data_messages.h"

class ChannelData;

namespace Data {

class Session;
class Feed;

enum class FeedUpdateFlag {
	Channels,
	ChannelPhoto,
};

struct FeedUpdate {
	not_null<Data::Feed*> feed;
	FeedUpdateFlag flag;
};

MessagePosition FeedPositionFromMTP(const MTPFeedPosition &position);

class Feed : public Dialogs::Entry {
public:
	static constexpr auto kId = 1;

	Feed(FeedId id, not_null<Data::Session*> parent);

	FeedId id() const;
	void registerOne(not_null<ChannelData*> channel);
	void unregisterOne(not_null<ChannelData*> channel);

	void updateLastMessage(not_null<HistoryItem*> item);
	void messageRemoved(not_null<HistoryItem*> item);
	void historyCleared(not_null<History*> history);

	void setUnreadCounts(int unreadCount, int unreadMutedCount);
	void setUnreadPosition(const MessagePosition &position);
	MessagePosition unreadPosition() const;
	rpl::producer<MessagePosition> unreadPositionChanges() const;

	bool toImportant() const override;
	bool shouldBeInChatList() const override;
	int chatListUnreadCount() const override;
	bool chatListMutedBadge() const override;
	HistoryItem *chatsListItem() const override;
	const QString &chatsListName() const override;
	const base::flat_set<QString> &chatsListNameWords() const override;
	const base::flat_set<QChar> &chatsListFirstLetters() const override;

	void loadUserpic() override;
	void paintUserpic(
		Painter &p,
		int x,
		int y,
		int size) const override;

	const std::vector<not_null<History*>> &channels() const;

private:
	void indexNameParts();
	void recountLastMessage();
	bool justSetLastMessage(not_null<HistoryItem*> item);

	FeedId _id = 0;
	not_null<Data::Session*> _parent;
	std::vector<not_null<History*>> _channels;

	QString _name;
	base::flat_set<QString> _nameWords;
	base::flat_set<QChar> _nameFirstLetters;
	HistoryItem *_lastMessage = nullptr;

	rpl::variable<MessagePosition> _unreadPosition;
	int _unreadCount = 0;
	int _unreadMutedCount = 0;

};

} // namespace Data