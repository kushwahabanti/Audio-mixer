class HistoryManager {
    constructor(list, historyPanelId) {
        this.list = list;
        this.historyPanel = document.getElementById(historyPanelId);
    }

    render() {
        this.historyPanel.innerHTML = '';
        const history = this.list.getHistory();
        
        if (history.length === 0) {
            this.historyPanel.innerHTML = '<div class="empty-state">No edits in history.</div>';
            return;
        }

        let isFuture = false;
        let foundCurrent = false;

        history.forEach((node, index) => {
            const isCurrent = (node === this.list.current);
            
            if (foundCurrent && this.list.current !== null) {
                isFuture = true;
            }

            if (isCurrent) {
                foundCurrent = true;
            }

            // If current is null, all nodes are "future" because we are at the initial state
            if (!this.list.current && history.length > 0) {
                isFuture = true;
            }

            const item = document.createElement('div');
            item.className = `history-item ${isCurrent ? 'active' : ''} ${isFuture ? 'undone' : ''}`;
            
            item.innerHTML = `
                <div class="edit-info">
                    <span class="edit-number">#${index + 1}</span>
                    <span>Vol: ${(node.oldVolume * 100).toFixed(0)}% &rarr; ${(node.newVolume * 100).toFixed(0)}%</span>
                </div>
                <div class="edit-time">${node.timestamp}</div>
            `;
            
            this.historyPanel.appendChild(item);
        });
    }
}
