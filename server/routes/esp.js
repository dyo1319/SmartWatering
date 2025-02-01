const express = require('express');
const router = express.Router();
const pool = require('../models/database');

router.post('/manual', (req, res) => {
    const { command } = req.body; // "on" or "off"
    setTimeout(() => {
        console.log(`Pump turned ${command}`);
        res.json({ success: true, message: `Pump turned ${command}` });
    }, 3000);
});


router.get('/state', (req, res) => {
    let data = JSON.parse(fs.readFileSync("Inside_information.json","utf8"));
    res.json(data.state);
});


module.exports = router;