child: Column(
children: [
// slivers:[
Row(
mainAxisAlignment: MainAxisAlignment.start,

// padding: const EdgeInsets.symmetric(
//     horizontal: 8, vertical: 16
// ),
children: [
Expanded(
child: TextFormField(
style: kSensorInputText,
decoration: InputDecoration(
border: UnderlineInputBorder(),
labelText: 'Enter Hardware Info',
labelStyle: kConfigTitle,
),
),
),
],
),
Row(
mainAxisAlignment: MainAxisAlignment.start,

// padding: const EdgeInsets.symmetric(
//     horizontal: 8, vertical: 16
// ),
children: [
Expanded(
child: TextFormField(
keyboardType: TextInputType.number,
style: kSensorInputText,
decoration: InputDecoration(
border: UnderlineInputBorder(),
labelText: 'Enter Sample Interval',
labelStyle: kConfigTitle,
),
),
),
],
),
Card(
child: Column(
children: [

],
)
),
Row(mainAxisAlignment: MainAxisAlignment.end, children: [
IconButton(
onPressed: () async {
setState(() {
_typesAndUnitsCount++;
});
},
icon: Icon(
Icons.add_circle,
color: Colors.blue,
),
),
SizedBox(
width: 10,
),
IconButton(
onPressed: () async {
setState(() {
if (_typesAndUnitsCount > 1) {
_typesAndUnitsCount--;
}
});
},
icon: Icon(
Icons.remove_circle,
color: Colors.red,
),
),
]),

Flexible(
child: ListView.builder(
shrinkWrap: true,
itemCount: _typesAndUnitsCount,
itemBuilder: (BuildContext ctx, int index) {
// int types_num = sensors_list[index].sensor_type!.length;
// List<List<String>> type_unit_list = [];

//Add sensor's text fields
return Row(
mainAxisAlignment: MainAxisAlignment.start,

// padding: const EdgeInsets.symmetric(
//     horizontal: 8, vertical: 16
// ),
children: [
Expanded(
child: TextFormField(
style: kSensorInputText,
decoration: InputDecoration(
border: UnderlineInputBorder(),
labelText: 'Enter Type',
labelStyle: kConfigTitle,
),
),
),
Expanded(
child: TextFormField(
style: kSensorInputText,
decoration: InputDecoration(
border: UnderlineInputBorder(),
labelText: 'Enter Units',
labelStyle: kConfigTitle,
),
),
),
],
);

// return Padding(
//   // padding: const EdgeInsets.symmetric(vertical: 10.0),
//   padding: const EdgeInsets.all(12.0),
//   child: Column(
//       crossAxisAlignment: CrossAxisAlignment.start,
//       children: sensorFields),
// );
},
),
),

// SizedBox(height: 10,),
Row(mainAxisAlignment: MainAxisAlignment.end, children: [
IconButton(
onPressed: () async {
setState(() {
_pinoutsCount++;
});
},
icon: Icon(
Icons.add_circle,
color: Colors.blue,
),
),
SizedBox(
width: 10,
),
IconButton(
onPressed: () async {
setState(() {
if (_pinoutsCount > 1) _pinoutsCount--;
});
},
icon: Icon(
Icons.remove_circle,
color: Colors.red,
),
),
]),
Flexible(
child: ListView.builder(
shrinkWrap: true,
itemCount: _pinoutsCount,
itemBuilder: (BuildContext ctx, int index) {
// int types_num = sensors_list[index].sensor_type!.length;
// List<List<String>> type_unit_list = [];

//Add sensor's text fields
return Row(
mainAxisAlignment: MainAxisAlignment.start,

// padding: const EdgeInsets.symmetric(
//     horizontal: 8, vertical: 16
// ),
children: [
Expanded(
child: TextFormField(
style: kSensorInputText,
decoration: InputDecoration(
border: UnderlineInputBorder(),
labelText: 'Enter Sensor Pin',
labelStyle: kConfigTitle,
),
),
),
Expanded(
child: TextFormField(
style: kSensorInputText,
decoration: InputDecoration(
border: UnderlineInputBorder(),
labelText: 'Enter Controller Pin',
labelStyle: kConfigTitle,
),
),
),
],
);

// return Padding(
//   // padding: const EdgeInsets.symmetric(vertical: 10.0),
//   padding: const EdgeInsets.all(12.0),
//   child: Column(
//       crossAxisAlignment: CrossAxisAlignment.start,
//       children: sensorFields),
// );
},
),
),
],
),