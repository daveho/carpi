#ifndef PLAYBACK_SETTINGS
#define PLAYBACK_SETTINGS

class PlaybackSettings {
private:
	int m_volumePercent;

public:
	PlaybackSettings();
	PlaybackSettings(const PlaybackSettings& other);
	virtual ~PlaybackSettings();

	PlaybackSettings &operator=(const PlaybackSettings &rhs);

	void setVolumePercent(int volumePercent);
	int getVolumePercent() const;

	PlaybackSettings *clone() const;

	bool operator==(const PlaybackSettings &rhs) const;
	bool operator!=(const PlaybackSettings &rhs) const { return !(*this == rhs); }
};

#endif // PLAYBACK_SETTINGS
