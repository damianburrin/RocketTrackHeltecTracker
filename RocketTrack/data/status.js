
function DrawSatPlot()
{
	var canvas = document.getElementById("satelliteplot");
	var ctx=canvas.getContext("2d");
	
//	ctx.font="100px Comic Sans MS";
//	ctx.fillStyle = "red";
//	ctx.textAlign = "center";
//	ctx.fillText("Hello World", canvas.width/2, canvas.height/2);	

	ctx.beginPath();
	ctx.arc(250,250,75,0,2*Math.PI);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.arc(250,250,150,0,2*Math.PI);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.arc(250,250,225,0,2*Math.PI);
	ctx.stroke();
	
	ctx.moveTo(0,250);
	ctx.lineTo(500,250);
	ctx.stroke();
	
	ctx.moveTo(250,0);
	ctx.lineTo(250,500);
	ctx.stroke();
	
}

