const express = require('express');
const router = express.Router();
const pool = require('../models/database');
const fs = require('fs');


router.get('/state', async (req, res) => {
  try {
      const rawData = await fs.promises.readFile("Inside_information.json", "utf8");
      const data = JSON.parse(rawData);
      res.json({ state: data.state }); // Wrap in a JSON object
  } catch (error) {
      console.error("Error reading state:", error);
      res.status(500).json({ error: "Failed to read state" });
  }
});


// ---- POST:  מצב המשאבה(ON/OFF) ----
router.post('/manual', (req, res) => {
    try {
        const data = JSON.parse(fs.readFileSync("Inside_information.json", "utf8"));
        const { manual_command } = req.body;
  
        if (manual_command === "on" || manual_command === "off") {
            data.manual_command = manual_command;
            fs.writeFileSync("Inside_information.json", JSON.stringify(data, null, 4), "utf8");
            res.status(200).json({ message: `Pump command updated to ${manual_command}` });
        } else {
            res.status(400).json({ error: "Invalid manual command. Use 'on' or 'off'." });
        }
    } catch (error) {
        console.error("Error updating manual command:", error);
        res.status(500).json({ error: "Failed to update manual command." });
    }
});

// ---- GET: שליפת מצב המשאבה (ON/OFF) ----
router.get('/manual', (req, res) => {
    try {
        const data = JSON.parse(fs.readFileSync("Inside_information.json", "utf8"));
        res.status(200).json({ manual_command: data.manual_command });
    } catch (error) {
        console.error("Error fetching manual command:", error);
        res.status(500).json({ error: "Failed to fetch manual command." });
    }
});


module.exports = router;