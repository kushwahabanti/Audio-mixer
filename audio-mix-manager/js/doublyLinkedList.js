class DoublyLinkedList {
    constructor() {
        this.head = null;
        this.tail = null;
        this.current = null;
    }

    addEdit(oldVolume, newVolume) {
        this.clearRedoHistory(); // New edit removes future branch
        
        const newNode = new Node(oldVolume, newVolume);

        if (!this.head) {
            this.head = newNode;
            this.tail = newNode;
            this.current = newNode;
        } else {
            this.current.next = newNode;
            newNode.prev = this.current;
            this.tail = newNode;
            this.current = newNode;
        }
    }

    undo() {
        if (!this.current) return null; // Already at the start
        
        const targetVolume = this.current.oldVolume;
        this.current = this.current.prev;
        
        return targetVolume;
    }

    redo() {
        // If current is null but history exists, we redo the first node
        let nextNode = this.current ? this.current.next : this.head;
        
        if (!nextNode) return null; // Nothing to redo
        
        this.current = nextNode;
        return this.current.newVolume;
    }

    clearRedoHistory() {
        if (this.current) {
            this.current.next = null;
            this.tail = this.current;
        } else {
            this.head = null;
            this.tail = null;
        }
    }

    getHistory() {
        let historyArray = [];
        let temp = this.head;
        while (temp) {
            historyArray.push(temp);
            temp = temp.next;
        }
        return historyArray;
    }
}
