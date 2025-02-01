document.getElementById("configForm").addEventListener("submit", function(event) {
    event.preventDefault();

    const desiredTemp = document.getElementById("desiredTemp").value;
    const highTempDuration = document.getElementById("highTempDuration").value;
    const lowTempDuration = document.getElementById("lowTempDuration").value;

    fetch("http://localhost:4002/user/config/temperature", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            desired_temp: parseInt(desiredTemp),
            high_temp_duration: parseInt(highTempDuration),
            low_temp_duration: parseInt(lowTempDuration)
        })
    })
    .then(response => response.json())
    .then(data => {
        document.getElementById("status").innerText = data.message;
    })
    .catch(error => {
        document.getElementById("status").innerText = "Error sending data";
        console.error("Error:", error);
    });
});
