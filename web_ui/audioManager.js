class AudioEditNode {
    constructor(id, version, actionName, category, value, timestamp = null) {
        this.editId = id;
        this.version = version;
        this.actionName = actionName;
        this.category = category;
        this.value = value;
        this.timestamp = timestamp || this.currentTimestamp();
        this.prev = null;
        this.next = null;
    }

    currentTimestamp() {
        const now = new Date();
        return now.toISOString().replace('T', ' ').substring(0, 19);
    }
}

class DoublyLinkedList {
    constructor() {
        this.head = null;
        this.tail = null;
        this.size = 0;
    }

    insertAtEnd(node) {
        if (!this.head) {
            this.head = this.tail = node;
        } else {
            this.tail.next = node;
            node.prev = this.tail;
            this.tail = node;
        }
        this.size++;
    }

    deleteById(id) {
        let curr = this.head;
        while (curr) {
            if (curr.editId === id) {
                if (curr.prev) curr.prev.next = curr.next;
                else this.head = curr.next;

                if (curr.next) curr.next.prev = curr.prev;
                else this.tail = curr.prev;

                this.size--;
                return true;
            }
            curr = curr.next;
        }
        return false;
    }

    clearAll() {
        this.head = this.tail = null;
        this.size = 0;
    }

    deleteAfter(node) {
        if (!node) {
            this.clearAll();
            return;
        }
        let curr = node.next;
        while (curr) {
            this.size--;
            curr = curr.next;
        }
        node.next = null;
        this.tail = node;
    }

    countNodes() {
        return this.size;
    }
}

class UndoRedoManager {
    constructor() {
        this.editHistory = new DoublyLinkedList();
        this.current = null;
        this.nextId = 1;
        this.currentVersion = 1;
        this.loadFromStorage();
    }

    branchAfterUndo() {
        if (this.current && this.current.next) {
            this.editHistory.deleteAfter(this.current);
        } else if (!this.current && this.editHistory.head) {
            this.editHistory.clearAll();
        }
    }

    addEdit(action, category, value) {
        this.branchAfterUndo();
        const newNode = new AudioEditNode(this.nextId++, this.currentVersion++, action, category, value);
        this.editHistory.insertAtEnd(newNode);
        this.current = newNode;
        this.saveToStorage();
        return newNode;
    }

    undo() {
        if (!this.current) return false;
        this.current = this.current.prev;
        this.saveToStorage();
        return true;
    }

    redo() {
        if (!this.current) {
            if (this.editHistory.head) {
                this.current = this.editHistory.head;
                this.saveToStorage();
                return true;
            }
            return false;
        }
        if (this.current.next) {
            this.current = this.current.next;
            this.saveToStorage();
            return true;
        }
        return false;
    }

    deleteById(id) {
        const isCurrent = (this.current && this.current.editId === id);
        const deleted = this.editHistory.deleteById(id);
        if (deleted && isCurrent) {
            this.current = this.editHistory.tail;
        }
        if (deleted) this.saveToStorage();
        return deleted;
    }

    getHistoryList() {
        const history = [];
        let curr = this.editHistory.head;
        while (curr) {
            history.push({
                node: curr,
                isCurrent: (curr === this.current)
            });
            curr = curr.next;
        }
        return history;
    }

    saveToStorage() {
        const data = [];
        let curr = this.editHistory.head;
        while (curr) {
            data.push({
                id: curr.editId,
                version: curr.version,
                action: curr.actionName,
                category: curr.category,
                value: curr.value,
                timestamp: curr.timestamp
            });
            curr = curr.next;
        }
        const state = {
            data,
            currentId: this.current ? this.current.editId : null,
            nextId: this.nextId,
            currentVersion: this.currentVersion
        };
        localStorage.setItem('audioManagerData', JSON.stringify(state));
    }

    loadFromStorage() {
        const raw = localStorage.getItem('audioManagerData');
        if (!raw) return;
        const state = JSON.parse(raw);
        
        this.nextId = state.nextId;
        this.currentVersion = state.currentVersion;
        
        state.data.forEach(item => {
            const node = new AudioEditNode(item.id, item.version, item.action, item.category, item.value, item.timestamp);
            this.editHistory.insertAtEnd(node);
            if (item.id === state.currentId) {
                this.current = node;
            }
        });
        
        if (state.currentId === null) {
            this.current = null;
        }
    }

    clearStorage() {
        localStorage.removeItem('audioManagerData');
        this.editHistory.clearAll();
        this.current = null;
        this.nextId = 1;
        this.currentVersion = 1;
    }
}
