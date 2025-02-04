document.getElementById("configForm").addEventListener("submit", function(event) {
    event.preventDefault();

    const desiredTemp = document.getElementById("desiredTemp").value;
    const highTempDuration = document.getElementById("highTempDuration").value;
    const lowTempDuration = document.getElementById("lowTempDuration").value;

    if (!desiredTemp || !highTempDuration || !lowTempDuration) {
        document.getElementById("status").innerText = "נא למלא את כל השדות!";
        return;
    }

    fetch("http://localhost:4002/user/config/temperature", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
            desired_temp: parseInt(desiredTemp),
            high_temp_duration: parseInt(highTempDuration),
            low_temp_duration: parseInt(lowTempDuration)
        })
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById("status").innerText = data.message || "שגיאה!";
    })
    .catch(error => {
        document.getElementById("status").innerText = "Error sending data";
        console.error("Error:", error);
    });
});

// Handle pump ON/OFF buttons
async function togglePump(command) {
    try {
        const response = await fetch("http://localhost:4002/esp/manual", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ manual_command: command })
        });

        if (!response.ok) throw new Error("Request failed");

        const data = await response.json();
        document.getElementById("manualStatus").innerText = 
            command === "on" ? "משאבה הופעלה בהצלחה!" : "משאבה כובתה בהצלחה!";
    } catch (error) {
        document.getElementById("manualStatus").innerText = "שגיאה בתהליך!";
        console.error(error);
    }
}

document.getElementById("turnOn").addEventListener("click", () => togglePump("on"));
document.getElementById("turnOff").addEventListener("click", () => togglePump("off"));
