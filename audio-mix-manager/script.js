document.addEventListener('DOMContentLoaded', () => {
    // Initialize Core Components
    const linkedList = new DoublyLinkedList();
    const hiddenAudioPlayer = document.getElementById('hiddenAudioPlayer');
    const audioManager = new AudioManager(hiddenAudioPlayer);
    const historyManager = new HistoryManager(linkedList, 'historyPanel');

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

    let isMuted = false;

    // Helper: Update UI Controls
    function updateUI() {
        historyManager.render();
        
        undoBtn.disabled = !linkedList.current;
        
        // Redo is enabled if there is a next node from current, or if current is null but history exists
        if (!linkedList.current && linkedList.head) {
            redoBtn.disabled = false;
        } else if (linkedList.current && linkedList.current.next) {
            redoBtn.disabled = false;
        } else {
            redoBtn.disabled = true;
        }
    }

    // Helper: Set Volume (Both Audio and Slider)
    function setSystemVolume(newVol) {
        audioManager.setVolume(newVol);
        volumeSlider.value = newVol;
        volumeStatus.textContent = `Current Volume: ${(newVol * 100).toFixed(0)}%`;
    }

    // 1. Upload Logic
    audioUpload.addEventListener('change', (e) => {
        const file = e.target.files[0];
        if (file) {
            uploadText.textContent = file.name;
            const objectUrl = URL.createObjectURL(file);
            audioManager.loadAudio(objectUrl);
            
            // Initial log
            linkedList.clearRedoHistory();
            linkedList.addEdit(1.0, 1.0); // Baseline
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

    volumeSlider.addEventListener('change', (e) => {
        const newVolume = parseFloat(e.target.value);
        if (newVolume !== lastVolume) {
            setSystemVolume(newVolume);
            linkedList.addEdit(lastVolume, newVolume);
            lastVolume = newVolume;
            updateUI();
        }
    });

    volumeSlider.addEventListener('input', (e) => {
        // Real-time update for audio player and text, but don't log to history yet
        const tempVol = parseFloat(e.target.value);
        audioManager.setVolume(tempVol);
        volumeStatus.textContent = `Current Volume: ${(tempVol * 100).toFixed(0)}%`;
    });

    // 4. Undo / Redo Logic
    function handleUndo() {
        if (undoBtn.disabled) return;
        const previousVolume = linkedList.undo();
        if (previousVolume !== null) {
            setSystemVolume(previousVolume);
            lastVolume = previousVolume;
            updateUI();
        }
    }

    function handleRedo() {
        if (redoBtn.disabled) return;
        const nextVolume = linkedList.redo();
        if (nextVolume !== null) {
            setSystemVolume(nextVolume);
            lastVolume = nextVolume;
            updateUI();
        }
    }

    undoBtn.addEventListener('click', handleUndo);
    redoBtn.addEventListener('click', handleRedo);

    // 5. Reset Project
    resetBtn.addEventListener('click', () => {
        if (confirm("Are you sure you want to reset the project?")) {
            audioManager.stop();
            audioUpload.value = '';
            uploadText.textContent = "Choose MP3/WAV file...";
            
            linkedList.head = null;
            linkedList.tail = null;
            linkedList.current = null;
            
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
