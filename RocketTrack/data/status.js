
function DrawSatPlot()
{
	var canvas = document.getElementById("satelliteplot");
	var ctx=canvas.getContext("2d");
	
//	ctx.font="100px Comic Sans MS";
//	ctx.fillStyle = "red";
//	ctx.textAlign = "center";
//	ctx.fillText("Hello World", canvas.width/2, canvas.height/2);	

	ctx.translate(250,250);
	
	ctx.beginPath();
	ctx.arc(0,0,75,0,2*Math.PI);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.arc(0,0,150,0,2*Math.PI);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.arc(0,0,225,0,2*Math.PI);
	ctx.stroke();
	
	ctx.moveTo(0,-250);
	ctx.lineTo(0,250);
	ctx.stroke();
	
	ctx.moveTo(-250,0);
	ctx.lineTo(250,0);
	ctx.stroke();
	
	SatSpotAt(ctx,45,60,123);
	
	
	
	
	
	
	
	
	
	
	
	
	
	DrawSatBars();
}

function SatSpotAt(ctx,az,el,num)
{
	console.log(Math.cos(az*Math.PI/180));
	
	var radius=225*(90-el)/90;
	
	ctx.beginPath();
	ctx.fillStyle="#008000";
	ctx.arc(Math.cos(az*Math.PI/180)*radius,Math.sin(az*Math.PI/180)*radius,15,0,2*Math.PI);
//	ctx.stroke();
	ctx.fill();
	
	var sathoriz=25;
	var satvert=10;
	
	ctx.strokeStyle="#008000";
	ctx.beginPath();
	ctx.moveTo(Math.cos(az*Math.PI/180)*radius-sathoriz,Math.sin(az*Math.PI/180)*radius);
	ctx.lineTo(Math.cos(az*Math.PI/180)*radius+sathoriz,Math.sin(az*Math.PI/180)*radius);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.moveTo(Math.cos(az*Math.PI/180)*radius-sathoriz,Math.sin(az*Math.PI/180)*radius-satvert);
	ctx.lineTo(Math.cos(az*Math.PI/180)*radius-sathoriz,Math.sin(az*Math.PI/180)*radius+satvert);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.moveTo(Math.cos(az*Math.PI/180)*radius+sathoriz,Math.sin(az*Math.PI/180)*radius-satvert);
	ctx.lineTo(Math.cos(az*Math.PI/180)*radius+sathoriz,Math.sin(az*Math.PI/180)*radius+satvert);
	ctx.stroke();
}

function DrawSatBars()
{
	var canvas = document.getElementById("satellitebars");
	var ctx=canvas.getContext("2d");
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
