class AudioManager {
    constructor(audioElement) {
        this.audioElement = audioElement;
        this.currentVolume = 1.0;
        this.audioElement.volume = this.currentVolume;
    }

    loadAudio(url) {
        this.audioElement.src = url;
        this.audioElement.load();
    }

    play() {
        if (this.audioElement.src) {
            this.audioElement.play();
        }
    }

    pause() {
        this.audioElement.pause();
    }

    stop() {
        this.audioElement.pause();
        this.audioElement.currentTime = 0;
    }

    mute(isMuted) {
        this.audioElement.muted = isMuted;
    }

    setVolume(volumeLevel) {
        this.currentVolume = Math.max(0, Math.min(1, volumeLevel));
        this.audioElement.volume = this.currentVolume;
    }

    getVolume() {
        return this.currentVolume;
    }
}
