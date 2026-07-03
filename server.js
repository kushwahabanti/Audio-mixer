const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const path = require('path');

const app = express();
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname))); // Serve static frontend files

let cppProcess = null;

// Initialize C++ Process
function startCppProcess() {
    if (cppProcess) {
        cppProcess.kill();
    }
    const isWindows = process.platform === 'win32';
    const executableName = isWindows ? 'audio_dsa.exe' : 'audio_dsa';
    
    cppProcess = spawn(path.join(__dirname, executableName));
    
    cppProcess.stderr.on('data', (data) => {
        console.error(`C++ Error: ${data}`);
    });
}

startCppProcess();

// Helper to send command to C++ and wait for response
function sendCommand(command) {
    return new Promise((resolve) => {
        const onData = (data) => {
            cppProcess.stdout.removeListener('data', onData);
            resolve(data.toString().trim());
        };
        cppProcess.stdout.on('data', onData);
        cppProcess.stdin.write(command + '\n');
    });
}

// API Routes
app.post('/api/add', async (req, res) => {
    const { oldVol, newVol, timestamp } = req.body;
    // Format: ADD oldVol newVol timestamp
    const response = await sendCommand(`ADD ${oldVol} ${newVol} ${timestamp}`);
    res.json({ status: response });
});

app.get('/api/undo', async (req, res) => {
    const response = await sendCommand('UNDO');
    res.json({ volume: parseFloat(response) });
});

app.get('/api/redo', async (req, res) => {
    const response = await sendCommand('REDO');
    res.json({ volume: parseFloat(response) });
});

app.get('/api/history', async (req, res) => {
    const response = await sendCommand('GET');
    try {
        res.json(JSON.parse(response));
    } catch(e) {
        res.status(500).json({ error: "Failed to parse history" });
    }
});

app.post('/api/reset', async (req, res) => {
    const response = await sendCommand('RESET');
    res.json({ status: response });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
    console.log(`Web UI available at http://localhost:${PORT}/index.html`);
});
