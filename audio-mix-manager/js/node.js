class Node {
    constructor(oldVolume, newVolume) {
        this.oldVolume = oldVolume;
        this.newVolume = newVolume;
        this.timestamp = new Date().toLocaleTimeString();
        this.prev = null;
        this.next = null;
    }
}
