const manager = new UndoRedoManager();

// DOM Elements
const form = document.getElementById('addEditForm');
const actionInput = document.getElementById('actionInput');
const categoryInput = document.getElementById('categoryInput');
const valueInput = document.getElementById('valueInput');
const historyList = document.getElementById('historyList');
const undoBtn = document.getElementById('undoBtn');
const redoBtn = document.getElementById('redoBtn');
const clearBtn = document.getElementById('clearBtn');
const searchInput = document.getElementById('searchInput');
const versionTracker = document.getElementById('versionTracker');
const totalEditsTracker = document.getElementById('totalEditsTracker');
const toastEl = document.getElementById('toast');

// Audio Player DOM
const audioUpload = document.getElementById('audioUpload');
const audioPlayer = document.getElementById('audioPlayer');
const uploadText = document.getElementById('uploadText');

function showToast(message) {
    toastEl.textContent = message;
    toastEl.classList.add('show');
    setTimeout(() => {
        toastEl.classList.remove('show');
    }, 3000);
}

function renderTimeline(filterQuery = '') {
    const history = manager.getHistoryList();
    historyList.innerHTML = '';
    
    if (history.length === 0) {
        historyList.innerHTML = `
            <div class="empty-state">
                <p>No edits yet. Add your first audio edit on the left.</p>
            </div>
        `;
        updateControls();
        return;
    }

    // Determine the split between past and future (for undone items)
    let foundCurrent = false;
    // We want to render latest at the bottom or top? Let's do chronological top to bottom.
    
    history.forEach((item) => {
        const node = item.node;
        
        // Filtering
        if (filterQuery) {
            const q = filterQuery.toLowerCase();
            if (!node.actionName.toLowerCase().includes(q) && !node.category.toLowerCase().includes(q)) {
                return; // Skip rendering
            }
        }

        const isCurrent = item.isCurrent;
        let isFuture = false;
        
        if (foundCurrent && manager.current !== null) {
            isFuture = true; // Nodes after current are undone
        }
        
        if (isCurrent) {
            foundCurrent = true;
        }

        const el = document.createElement('div');
        el.className = `history-node ${isCurrent ? 'is-current' : ''} ${isFuture ? 'is-future' : ''}`;
        if (!manager.current && history.length > 0) {
            // if current is null but history exists, everything is undone/future
            el.classList.add('is-future'); 
        }

        el.innerHTML = `
            <div class="node-header">
                <div class="node-title">
                    <h4>${node.actionName}</h4>
                    <span class="node-category">${node.category}</span>
                </div>
                <div class="node-meta">V${node.version} &bull; ${node.timestamp}</div>
            </div>
            <div class="node-body">
                <div class="node-value">${node.value}</div>
                <button class="delete-btn" data-id="${node.editId}">Delete</button>
            </div>
        `;

        el.querySelector('.delete-btn').addEventListener('click', () => {
            if (manager.deleteById(node.editId)) {
                showToast(`Deleted Edit V${node.version}`);
                renderTimeline(searchInput.value);
            }
        });

        historyList.appendChild(el);
    });

    updateControls();
}

function updateControls() {
    const history = manager.getHistoryList();
    const hasHistory = history.length > 0;
    
    undoBtn.disabled = !manager.current;
    
    // Can redo if current is not the last item, or if current is null but history exists
    if (!manager.current && hasHistory) {
        redoBtn.disabled = false;
    } else if (manager.current && manager.current.next) {
        redoBtn.disabled = false;
    } else {
        redoBtn.disabled = true;
    }

    const currentV = manager.current ? manager.current.version : 0;
    versionTracker.textContent = `Current Version: V${currentV}`;
    totalEditsTracker.textContent = `Total Edits: ${manager.editHistory.countNodes()}`;
}

// Event Listeners
form.addEventListener('submit', (e) => {
    e.preventDefault();
    const action = actionInput.value.trim();
    const cat = categoryInput.value;
    const val = valueInput.value.trim();
    
    if (action && val) {
        manager.addEdit(action, cat, val);
        actionInput.value = '';
        valueInput.value = '';
        actionInput.focus();
        showToast('Edit Added');
        
        // Clear search on new add
        searchInput.value = '';
        renderTimeline();
    }
});

undoBtn.addEventListener('click', () => {
    if (manager.undo()) {
        showToast('Undo Successful');
        renderTimeline(searchInput.value);
    }
});

redoBtn.addEventListener('click', () => {
    if (manager.redo()) {
        showToast('Redo Successful');
        renderTimeline(searchInput.value);
    }
});

clearBtn.addEventListener('click', () => {
    if (confirm('Are you sure you want to clear all history?')) {
        manager.clearStorage();
        showToast('History Cleared');
        renderTimeline();
    }
});

searchInput.addEventListener('input', (e) => {
    renderTimeline(e.target.value);
});

// Keyboard shortcuts
document.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'z') {
        e.preventDefault();
        undoBtn.click();
    }
    if (e.ctrlKey && e.key === 'y') {
        e.preventDefault();
        redoBtn.click();
    }
});

// Init
renderTimeline();

// Audio Upload Logic
audioUpload.addEventListener('change', function(e) {
    const file = e.target.files[0];
    if (file) {
        const objectUrl = URL.createObjectURL(file);
        audioPlayer.src = objectUrl;
        audioPlayer.style.display = 'block';
        uploadText.textContent = file.name;
        showToast('Audio loaded: ' + file.name);
    }
});
