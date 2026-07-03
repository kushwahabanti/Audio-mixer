document.addEventListener('DOMContentLoaded', () => {
    // Initialize Core Components
    const hiddenAudioPlayer = document.getElementById('hiddenAudioPlayer');
    const audioManager = new AudioManager(hiddenAudioPlayer);

    // DOM Elements
    const audioUpload = document.getElementById('audioUpload');
    const uploadText = document.getElementById('uploadText');
    const playBtn = document.getElementById('playBtn');
    const pauseBtn = document.getElementById('pauseBtn');
    const stopBtn = document.getElementById('stopBtn');
    const muteBtn = document.getElementById('muteBtn');
    const volumeSlider = document.getElementById('volumeSlider');
    const volumeStatus = document.getElementById('volumeStatus');
    const undoBtn = document.getElementById('undoBtn');
    const redoBtn = document.getElementById('redoBtn');
    const resetBtn = document.getElementById('resetBtn');
    const historyPanel = document.getElementById('historyPanel');

    let isMuted = false;

    // Fetch History and Update UI
    async function updateUI() {
        try {
            const res = await fetch('/api/history');
            const data = await res.json();
            renderHistory(data.history, data.currentIsNull);
        } catch (e) {
            console.error("Error fetching history", e);
        }
    }

    function renderHistory(history, currentIsNull) {
        historyPanel.innerHTML = '';
        
        if (history.length === 0) {
            historyPanel.innerHTML = '<div class="empty-state">No edits in history.</div>';
            undoBtn.disabled = true;
            redoBtn.disabled = true;
            return;
        }

        let isFuture = false;
        let foundCurrent = false;
        let hasNext = false;

        history.forEach((node, index) => {
            if (foundCurrent && !node.isCurrent) {
                isFuture = true;
            }
            if (node.isCurrent) {
                foundCurrent = true;
                if (index < history.length - 1) hasNext = true;
            }
            if (currentIsNull && history.length > 0) {
                isFuture = true;
                hasNext = true; // Can redo the first item
            }

            const item = document.createElement('div');
            item.className = `history-item ${node.isCurrent ? 'active' : ''} ${isFuture ? 'undone' : ''}`;
            
            item.innerHTML = `
                <div class="edit-info">
                    <span class="edit-number">#${index + 1}</span>
                    <span>Vol: ${(node.oldVolume * 100).toFixed(0)}% &rarr; ${(node.newVolume * 100).toFixed(0)}%</span>
                </div>
                <div class="edit-time">${node.timestamp}</div>
            `;
            historyPanel.appendChild(item);
        });

        undoBtn.disabled = currentIsNull;
        redoBtn.disabled = !hasNext;
    }

    // Helper: Set Volume
    function setSystemVolume(newVol) {
        audioManager.setVolume(newVol);
        volumeSlider.value = newVol;
        volumeStatus.textContent = `Current Volume: ${(newVol * 100).toFixed(0)}%`;
    }

    // 1. Upload Logic
    audioUpload.addEventListener('change', async (e) => {
        const file = e.target.files[0];
        if (file) {
            uploadText.textContent = file.name;
            const objectUrl = URL.createObjectURL(file);
            audioManager.loadAudio(objectUrl);
            
            await fetch('/api/reset', { method: 'POST' });
            
            const timestamp = new Date().toLocaleTimeString();
            await fetch('/api/add', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ oldVol: 1.0, newVol: 1.0, timestamp })
            });

            setSystemVolume(1.0);
            updateUI();
        }
    });

    // 2. Playback Controls
    playBtn.addEventListener('click', () => audioManager.play());
    pauseBtn.addEventListener('click', () => audioManager.pause());
    stopBtn.addEventListener('click', () => audioManager.stop());
    
    muteBtn.addEventListener('click', () => {
        isMuted = !isMuted;
        audioManager.mute(isMuted);
        muteBtn.textContent = isMuted ? "Unmute" : "Mute";
        muteBtn.classList.toggle('danger-btn', !isMuted);
        muteBtn.classList.toggle('secondary-btn', isMuted);
    });

    // 3. Volume Editing (Triggers History)
    let lastVolume = 1.0;
    
    volumeSlider.addEventListener('mousedown', () => {
        lastVolume = parseFloat(volumeSlider.value);
    });

    volumeSlider.addEventListener('change', async (e) => {
        const newVolume = parseFloat(e.target.value);
        if (newVolume !== lastVolume) {
            setSystemVolume(newVolume);
            
            const timestamp = new Date().toLocaleTimeString();
            await fetch('/api/add', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ oldVol: lastVolume, newVol: newVolume, timestamp })
            });

            lastVolume = newVolume;
            updateUI();
        }
    });

    volumeSlider.addEventListener('input', (e) => {
        const tempVol = parseFloat(e.target.value);
        audioManager.setVolume(tempVol);
        volumeStatus.textContent = `Current Volume: ${(tempVol * 100).toFixed(0)}%`;
    });

    // 4. Undo / Redo Logic
    async function handleUndo() {
        if (undoBtn.disabled) return;
        const res = await fetch('/api/undo');
        const data = await res.json();
        if (data.volume !== -1) {
            setSystemVolume(data.volume);
            lastVolume = data.volume;
            updateUI();
        }
    }

    async function handleRedo() {
        if (redoBtn.disabled) return;
        const res = await fetch('/api/redo');
        const data = await res.json();
        if (data.volume !== -1) {
            setSystemVolume(data.volume);
            lastVolume = data.volume;
            updateUI();
        }
    }

    undoBtn.addEventListener('click', handleUndo);
    redoBtn.addEventListener('click', handleRedo);

    // 5. Reset Project
    resetBtn.addEventListener('click', async () => {
        if (confirm("Are you sure you want to reset the project?")) {
            audioManager.stop();
            audioUpload.value = '';
            uploadText.textContent = "Choose MP3/WAV file...";
            
            await fetch('/api/reset', { method: 'POST' });
            
            setSystemVolume(1.0);
            updateUI();
        }
    });

    // 6. Keyboard Shortcuts
    document.addEventListener('keydown', (e) => {
        if (e.ctrlKey && e.key.toLowerCase() === 'z') {
            e.preventDefault();
            handleUndo();
        }
        if (e.ctrlKey && e.key.toLowerCase() === 'y') {
            e.preventDefault();
            handleRedo();
        }
    });

    // Initial UI Render
    updateUI();
});
