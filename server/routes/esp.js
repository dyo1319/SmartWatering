const express = require('express');
const router = express.Router();
const pool = require('../models/database');

router.get('/', (req,res) => {
    const {temp, linght, moisture} = req.query;
    console.log(linght);
})

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

module.exports = router;