
function DrawSatPlot()
{
//	ctx.font="100px Comic Sans MS";
//	ctx.fillStyle = "red";
//	ctx.textAlign = "center";
//	ctx.fillText("Hello World", canvas.width/2, canvas.height/2);	

	const sat_nums=[12,23];
	const sat_elevs=[60,5];
	const sat_azs=[45,170];
	const sat_snrs=[31,40];
	
	DrawSatCrosshairs();
	DrawSatSpots(sat_nums,sat_elevs,sat_azs,sat_snrs);
	DrawTextData();
	
	DrawSatBars(sat_nums,sat_elevs,sat_azs,sat_snrs);
}

function DrawSatCrosshairs()
{
	var canvas=document.getElementById("satelliteplot");
	var ctx=canvas.getContext("2d");
	
	ctx.translate(canvas.width/2,canvas.height/2);
	
	var radius=canvas.width*0.9/2;
	
	ctx.beginPath();
	ctx.arc(0,0,radius/3,0,2*Math.PI);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.arc(0,0,2*radius/3,0,2*Math.PI);
	ctx.stroke();
	
	ctx.beginPath();
	ctx.arc(0,0,radius,0,2*Math.PI);
	ctx.stroke();
	
	ctx.moveTo(0,-canvas.height/2);
	ctx.lineTo(0,canvas.height/2);
	ctx.stroke();
	
	ctx.moveTo(-canvas.width/2,0);
	ctx.lineTo(canvas.width/2,0);
	ctx.stroke();
}

function DrawSatSpots(sat_nums,sat_elevs,sat_azs,sat_snrs)
{
	var canvas=document.getElementById("satelliteplot");
	var ctx=canvas.getContext("2d");
	
	for(let cnt=0;cnt<sat_nums.length;cnt++)
	{
		SatSpotAt(canvas,ctx,sat_azs[cnt],sat_elevs[cnt],sat_nums[cnt]);
	}
}

function DrawTextData()
{
	var canvas=document.getElementById("satelliteplot");
	var ctx=canvas.getContext("2d");
	
	// these all need to be set by the ayncwebserver templating stuff
	
	var numCh=2;
	var gpsFix=0;
	var hAcc=30;
	
	var batCurrent=%BAT_CURRENT%;
	var batVolt=%BAT_VOLTAGE%;
	var batStatus=%BAT_STATUS%;
	
	var frequency=434.650;
	var loramode="%MODE_FROM_TEMPLATE%";
	
	var latitude="%LATITUDE%";
	var longitude="%LONGITUDE%";
	var altitude="%ALTITUDE%";
	
	ctx.fillStyle = "red";
	ctx.textAlign = "left";
	
	ctx.font="30px Comic Sans MS";
	ctx.fillText(numCh+" sats",-0.45*canvas.width,0.35*canvas.height);
	
	if(gpsFix==0)		ctx.fillText("No Fix",-0.45*canvas.width,0.40*canvas.height);
	else if(gpsFix==2)	ctx.fillText("2D Fix",-0.45*canvas.width,0.40*canvas.height);
	else				ctx.fillText("3D Fix",-0.45*canvas.width,0.40*canvas.height);
	
	ctx.fillText(hAcc+" m",-0.45*canvas.width,0.45*canvas.height);
	
	ctx.font="30px Comic Sans MS";
	ctx.fillText("V:"+batVolt+" V",-0.45*canvas.width,-0.45*canvas.height);
	ctx.fillText("I:"+(batCurrent*1000)+" mA",-0.45*canvas.width,-0.4*canvas.height);
	ctx.fillText(batStatus,-0.45*canvas.width,-0.35*canvas.height);
	
	ctx.textAlign = "right";
	ctx.font="30px Comic Sans MS";
	ctx.fillText(latitude,0.45*canvas.width,-0.45*canvas.height);
	ctx.fillText(longitude,0.45*canvas.width,-0.40*canvas.height);
	ctx.fillText(altitude+" m",0.45*canvas.width,-0.35*canvas.height);
	
	ctx.font="30px Comic Sans MS";
	ctx.fillText(frequency+" MHz",0.45*canvas.width,0.45*canvas.height);
	ctx.fillText(loramode,0.45*canvas.width,0.40*canvas.height);
}

function SatSpotAt(canvas,ctx,az,el,num)
{
	console.log(Math.cos(az*Math.PI/180));
	
	var radius=0.9*canvas.width/2*(90-el)/90;
	
	ctx.beginPath();
	ctx.fillStyle="#008000";
	ctx.arc(Math.cos(az*Math.PI/180)*radius,Math.sin(az*Math.PI/180)*radius,15,0,2*Math.PI);
	ctx.fill();
	
	var sathoriz=25;
	var satvert=10;
	
	ctx.strokeStyle="#008000";
	ctx.beginPath();
	ctx.moveTo(Math.cos(az*Math.PI/180)*radius-sathoriz,Math.sin(az*Math.PI/180)*radius);
	ctx.lineTo(Math.cos(az*Math.PI/180)*radius+sathoriz,Math.sin(az*Math.PI/180)*radius);
	ctx.lineWidth=3;
	ctx.stroke();
	
	ctx.beginPath();
	ctx.moveTo(Math.cos(az*Math.PI/180)*radius-sathoriz,Math.sin(az*Math.PI/180)*radius-satvert);
	ctx.lineTo(Math.cos(az*Math.PI/180)*radius-sathoriz,Math.sin(az*Math.PI/180)*radius+satvert);
	ctx.lineWidth=3;
	ctx.stroke();
	
	ctx.beginPath();
	ctx.moveTo(Math.cos(az*Math.PI/180)*radius+sathoriz,Math.sin(az*Math.PI/180)*radius-satvert);
	ctx.lineTo(Math.cos(az*Math.PI/180)*radius+sathoriz,Math.sin(az*Math.PI/180)*radius+satvert);
	ctx.lineWidth=3;
	ctx.stroke();
	
	ctx.font="20px Comic Sans MS";
	ctx.fillStyle="white";
	ctx.textAlign="center";
	ctx.fillText(num,Math.cos(az*Math.PI/180)*radius,Math.sin(az*Math.PI/180)*radius+7);
}

function DrawSatBars(sat_nums,sat_elevs,sat_azs,sat_snrs)
{
	var canvas=document.getElementById("satellitebars");
	var ctx=canvas.getContext("2d");
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
