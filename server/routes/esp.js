const express = require('express');
const router = express.Router();
const pool = require('../models/database');

// מצב נוכחי
let currentState = "TEMPERATURE"; // ברירת מחדל

router.get('/', (req,res) => {
    const {temp, linght, moisture} = req.query;
    console.log(linght);
})

// נקודת GET להחזרת המצב הנוכחי
router.get('/state', (req, res) => {
    res.json({ state: currentState });
});

// נקודת POST לעדכון המצב
router.post('/state', (req, res) => {
    const { state } = req.body;
    if (!["TEMPERATURE", "HUMIDITY", "SHABBAT", "MANUAL"].includes(state)) {
        return res.status(400).send("Invalid state");
    }
    currentState = state;
    res.send(`State updated to ${state}`);
});

/*
router.get('/state',async(req,res) => {
    try {
        const [rows] = await pool.query('SELECT * FROM threes WHERE id_plants = ?', [1]);
    
        if(rows.length > 0) {
            res.status(200).json(rows[0]);
        } else {
            res.status(404).send('no state found for  the specified plant ID');
        }
    } catch (err) {
        console.error('Error fetching state:', err);
        res.status(500).send('Error fetching state');
    }
});
*/

router.post('/sensor-data', async (req, res) => {
    const { id_sensor, id_plant, value } = req.body; 

    if (!id_sensor || !id_plant || value === undefined) {
        return res.status(400).send('Missing required fields: id_sensor, id_plant, value');
    }

    try {
        await pool.query(
            'INSERT INTO datasensor (id_sensors, id_threes, avg, date) VALUES (?, ?, ?, NOW())',
            [id_sensor, id_plant, value]
        );

        console.log(`Data saved: Sensor ID=${id_sensor}, Plant ID=${id_plant}, Value=${value}`);
        res.status(200).send('Sensor data saved successfully');
    } catch (err) {
        console.error('Error saving data:', err);
        res.status(500).send('Failed to save sensor data');
    }
});

module.exports = router;