const express = require('express');
const path = require('path');
const morgan = require('morgan');
const cors = require('cors');
const dotenv = require('dotenv');
const app = express();
const fs = require('fs').promises;
const HTTP_PORT = 4002;

dotenv.config();

// Middleware
app.use(express.json());
app.use(cors());
app.use(morgan('dev'));
app.use(express.static(path.join(__dirname, 'public')));

// JSON FILE LOCATION
const filePath = path.join(__dirname, 'Inside_information.json');


async function saveDataToFile(data) {
    await fs.writeFile(filePath, JSON.stringify(data, null, 4), 'utf8');
}

app.post('/user/config/temperature', async (req, res) => {
    const { desired_temp, high_temp_duration, low_temp_duration } = req.body;

    if (!desired_temp || !high_temp_duration || !low_temp_duration) {
        return res.status(400).json({ error: "Missing data fields!" });
    }

    try {
        let data = {};
        if (await fs.readFile(filePath, 'utf8')) {
            data = JSON.parse(await fs.readFile(filePath, 'utf8'));
        }

        data.temperature_config = { desired_temp, high_temp_duration, low_temp_duration };
        await saveDataToFile(data);
        res.json({ success: true, message: "Data saved successfully!" });
    } catch (error) {
        console.error("Error saving data:", error);
        res.status(500).json({ error: "Internal server error" });
    }
});

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

const esp = require('./routes/esp');
app.use('/esp', esp);


app.listen(HTTP_PORT, () => {
    console.log(`The server is running on port: ${HTTP_PORT} \nlink: http://localhost:${HTTP_PORT}`);
});
