
function DrawSatPlot()
{
	var canvas = document.getElementById("satelliteplot");
	var ctx=canvas.getContext("2d");
	ctx.font="100px Comic Sans MS";
	ctx.fillStyle = "red";
	ctx.textAlign = "center";
	ctx.fillText("Hello World", canvas.width/2, canvas.height/2);	
}

