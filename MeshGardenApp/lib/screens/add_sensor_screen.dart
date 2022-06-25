import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/model/Node.dart';
import 'package:iot_firestore_flutter_app/auth_helper.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/widgets/my_text_field.dart';
import 'package:iot_firestore_flutter_app/widgets/my_text_button.dart';
import 'package:adaptive_dialog/adaptive_dialog.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';

import 'package:iot_firestore_flutter_app/model/Sensor.dart';
import 'package:iot_firestore_flutter_app/route/routing_constants.dart';

//import 'package:iot_firestore_flutter_app/widgets/my_sensor_card.dart';
// import 'package:flutter_hooks/flutter_hooks.dart';


class AddSensorScreen extends StatefulWidget {
  const AddSensorScreen(
      {Key? key, required this.sensorDoc, required this.sensorId})
      : super(key: key);

  final DocumentReference<Map<String, dynamic>> sensorDoc;
  final int sensorId;

  @override
  _AddSensorScreenState createState() => _AddSensorScreenState();
}

// class _NodesScreenState extends State<NodesScreen> {
//
//   static String collectionName = 'TestNodes';
//   final nodeRef = FirebaseFirestore.instance
//       .collection(collectionName)
//       .withConverter<Node>(
//     fromFirestore: (snapshots, _) => Node.fromJson(snapshots.data()!),
//     toFirestore: (node, _) => node.toJson(),
//   );

class _AddSensorScreenState extends State<AddSensorScreen> {
  var _hardware_info = TextEditingController();
  var _sample_interval = TextEditingController();
  // Map<TextEditingController, TextEditingController> _pinouts = {};
  // Map<TextEditingController, TextEditingController> _typesAndUnits = {};

  // var _typeCon = TextEditingController();
  // var _unitsCon = TextEditingController();
  // var _sensorPinCon = TextEditingController();
  // var _controllerPinCon = TextEditingController();

  // List<TextEditingController>  _types= [];
  // List<TextEditingController> _units= [];
  // List<TextEditingController> _sensor_pins= [];
  // List<TextEditingController> _controller_pins= [];

  // List<Map<String, dynamic>> _pinouts = [];
  // List<Map<String, dynamic>> _sensor_types = [];
  // List<Map<String, dynamic>> _units = [];

  int _pinoutsCount = 1;
  int _typesAndUnitsCount = 1;

  Map<String, String> pinout_map ={};
  List<String> types_list = [];
  List<String> units_list = [];

  List<String> sensor_pin_list= [];
  List<String> controller_pin_list= [];

  // Sensor sensor = Sensor(
  //     hardware_info: "DHT22",
  //     pinout: {"DAT": "D1"},
  //     sample_interval: 30,
  //     sensor_id: 1,
  //     sensor_type: ["Air Humidity", "Air Temperature"],
  //     units: ["%", "C"]);

  // var sensor_json = sensor.toJson();

  // final TextEditingController _password = TextEditingController();
  // final TextEditingController _passwordConfirm = TextEditingController();


  @override
  void initState() {
    types_list.add("");
    units_list.add("");
    sensor_pin_list.add("");
    controller_pin_list.add("");
    super.initState();
  }

  @override
  void dispose() {
    _hardware_info.dispose();
    _sample_interval.dispose();

    // _passwordConfirm.dispose();
    super.dispose();
  }

  // final data = FirebaseFirestore.instance
  //     .collection("TestNodes").doc(widget.nodeId)

  // final nodeRef = FirebaseFirestore.instance
  //     .collection("TestNodes")
  //     .withConverter<Node>(
  //   fromFirestore: (snapshots, _) => Node.fromJson(snapshots.data()!),
  //   toFirestore: (node, _) => node.toJson(),
  // );

  /**
      @override
      Widget build(BuildContext context) {
      // print(_active.text);
      return Scaffold(
      appBar: AppBar(
      backgroundColor: kMainBG,
      elevation: 10.0,
      leading: IconButton(
      onPressed: () {
      Navigator.pop(context);
      },
      icon: Image(
      width: 24,
      color: Colors.white,
      image: AssetImage('assets/images/back_arrow.png'),
      ),
      ),
      title: Text("Add sensor", style: kBodyText2),
      ),
      body: Scaffold(
      body: Padding(
      padding:
      const EdgeInsets.only(left: 16, right: 16, top: 20, bottom: 30),
      // child: Column(
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
      ListTile(
      title: Text("Open Popup", style: kConfigTitle),
      onTap: () {
      showDialog(
      context: context,
      builder: (BuildContext context) {
      return AlertDialog(
      scrollable: true,
      title: Text('Login'),
      content: Padding(
      padding: const EdgeInsets.all(8.0),
      child: Column(
      children: <Widget>[
      Row(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
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
      if (_pinoutsCount > 1)
      _pinoutsCount--;
      });
      },
      icon: Icon(
      Icons.remove_circle,
      color: Colors.red,
      ),
      ),
      ]),
      Container (
      height: 300.0, // Change as per your requirement
      width: 300.0, // Change as per your requirement
      child: ListView.builder(
      shrinkWrap: true,
      itemCount: _pinoutsCount,
      itemBuilder: (BuildContext ctx, int index) {
      // int types_num = sensors_list[index].sensor_type!.length;
      // List<List<String>> type_unit_list = [];

      //Add sensor's text fields
      return Row(
      mainAxisAlignment:
      MainAxisAlignment.start,

      // padding: const EdgeInsets.symmetric(
      //     horizontal: 8, vertical: 16
      // ),
      children: [
      TextFormField(
      style: kSensorInputText,
      decoration: InputDecoration(
      border: UnderlineInputBorder(),
      labelText: 'Enter Sensor Pin',
      labelStyle: kConfigTitle,
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
      ),
      // actions: [
      //   RaisedButton(
      //       child: Text("Submit"),
      //       onPressed: () {
      //         // your code
      //       })
      // ],
      );
      });
      },
      ),

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
      ),
      ),
      );
      }
   */

  @override
  Widget build(BuildContext context) {
    bool keyboardIsOpened = MediaQuery.of(context).viewInsets.bottom != 0.0;


    return Scaffold(
      floatingActionButton: keyboardIsOpened? null: FloatingActionButton.extended(
        onPressed: (){
          try {
            // _doc.update({"sensors.1": sensor_json});
            // snapshot.requireData.reference.update({"sensors.1": sensor_json});
            // print(sensor_pin_list[0] + "...." + sensor_pin_list.length.toString() +"\n");
            // print(controller_pin_list[0] + "...." + controller_pin_list.length.toString() +"\n");
            //
            // print("//////////\n");
            // print(types_list[0] + "...." + types_list.length.toString() +"\n");
            // print(units_list[0] + "...." + units_list.length.toString() +"\n");

            for (int i = 0; i < sensor_pin_list.length; i++) {
              pinout_map[sensor_pin_list[i]] = controller_pin_list[i];
            }

            Sensor sensor = Sensor(
                hardware_info: _hardware_info.text.trim(),
                pinout: pinout_map,
                sample_interval: int.parse(_sample_interval.text.trim()),
                sensor_id: widget.sensorId,
                sensor_type: types_list,
                units: units_list);

            pinout_map ={};

            var sensor_json = sensor.toJson();



            widget.sensorDoc.update({"sensors.${widget.sensorId}": sensor_json});
            Navigator.pop(context);
            // print(sensor_json);
            // print("\n ***** ${_pinoutsCount} *** \n");
          }
          catch(e) {print("");}

        },
        // mini: true,

        elevation: 12,
        label: Text("Submit" , style: kSubmitTitle),
        backgroundColor: Colors.white,
        // foregroundColor: Colors.white,
        // child: Icon(Icons.ok,size: 30,),
      ),
      appBar: AppBar(
        backgroundColor: kMainBG,
        elevation: 10.0,
        leading: IconButton(
          onPressed: () {
            Navigator.pop(context);
          },
          icon: Image(
            width: 24,
            color: Colors.white,
            image: AssetImage('assets/images/back_arrow.png'),
          ),
        ),
        title: Text("Add sensor", style: kBodyText2),
      ),
      body: Scaffold(
        body: Padding(
          padding:
              const EdgeInsets.only(left: 16, right: 16, top: 20, bottom: 30),
          // child: Column(
          child: Padding(
            padding: const EdgeInsets.all(12.0),
            child: ListView(
              children: [
                // slivers:[
                Row(
                  mainAxisAlignment: MainAxisAlignment.start,

                  // padding: const EdgeInsets.symmetric(
                  //     horizontal: 8, vertical: 16
                  // ),
                  children: [
                    Flexible(
                      child: TextFormField(
                        style: kSensorInputText,
                        decoration: InputDecoration(
                          border: UnderlineInputBorder(),
                          labelText: 'Hardware Info',
                          labelStyle: kConfigTitle,
                        ),
                        controller: _hardware_info,
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
                    Flexible(
                      child: TextFormField(
                        keyboardType: TextInputType.number,
                        style: kSensorInputText,
                        decoration: InputDecoration(
                          border: UnderlineInputBorder(),
                          labelText: 'Sample Interval',
                          labelStyle: kConfigTitle,
                        ),
                        controller: _sample_interval,
                      ),
                    ),
                  ],
                ),
                Row(mainAxisAlignment: MainAxisAlignment.end, children: [
                  IconButton(
                    onPressed: () async {
                      setState(() {
                        _typesAndUnitsCount++;
                        types_list.add(_typesAndUnitsCount.toString());
                        units_list.add(_typesAndUnitsCount.toString());
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
                          types_list.removeLast();
                          units_list.removeLast();
                        }
                      });
                    },
                    icon: Icon(
                      Icons.remove_circle,
                      color: Colors.red,
                    ),
                  ),
                ]),

                  ListView.builder(
                    physics: ScrollPhysics(),
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
                          Flexible(
                            child: TextFormField(
                              // keyboardType: TextInputType.name,
                              style: kSensorInputText,
                              decoration: InputDecoration(
                                border: UnderlineInputBorder(),
                                labelText: 'Type',
                                labelStyle: kConfigTitle,
                              ),
                              // controller: _typeCon,
                              onChanged: (text){
                                types_list[index]= text.trim();
                                },
                            ),
                          ),
                          Flexible(
                            child: TextFormField(
                              style: kSensorInputText,
                              decoration: InputDecoration(
                                border: UnderlineInputBorder(),
                                labelText: 'Units',
                                labelStyle: kConfigTitle,
                              ),
                              onChanged: (text){
                                units_list[index]= text.trim();
                              },
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

                // SizedBox(height: 10,),
                Row(mainAxisAlignment: MainAxisAlignment.end, children: [
                  IconButton(
                    onPressed: () async {
                      setState(() {
                        _pinoutsCount++;
                        sensor_pin_list.add(_pinoutsCount.toString());
                        controller_pin_list.add(_pinoutsCount.toString());
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
                        if (_pinoutsCount > 1) {
                          _pinoutsCount--;
                          sensor_pin_list.removeLast();
                          controller_pin_list.removeLast();
                        }
                      });
                    },
                    icon: Icon(
                      Icons.remove_circle,
                      color: Colors.red,
                    ),
                  ),
                ]),
                ListView.builder(
                  physics: ScrollPhysics(),
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
                          Flexible(
                            child: TextFormField(
                              style: kSensorInputText,
                              decoration: InputDecoration(
                                border: UnderlineInputBorder(),
                                labelText: 'Sensor Pin',
                                labelStyle: kConfigTitle,
                              ),
                              onChanged: (text){
                                sensor_pin_list[index]= text.trim();
                              },
                            ),
                          ),
                          Flexible(
                            child: TextFormField(
                              style: kSensorInputText,
                              decoration: InputDecoration(
                                border: UnderlineInputBorder(),
                                labelText: 'Controller Pin',
                                labelStyle: kConfigTitle,
                              ),
                              onChanged: (text){
                                controller_pin_list[index]= text.trim();
                              },
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
                SizedBox(height: 40,),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Future _openEditDialog(keyboard_type, snap, control, type, current_data) =>
      showDialog(
        context: context,
        builder: (context) => AlertDialog(
          title: Text(current_data.toString()),
          content: TextField(
            keyboardType: keyboard_type,
            // onChanged: (value) {
            //   //save the _textFieldController.text to a variable
            // },
            controller: control,
            decoration: InputDecoration(hintText: type),
          ),
          actions: [
            TextButton(
              style: ButtonStyle(
                overlayColor: MaterialStateProperty.resolveWith(
                  (states) => Colors.black12,
                ),
              ),
              onPressed: () {
                String error = _updateData(snap, control, type);
                if (error != "no") {
                  showOkAlertDialog(
                    context: context,
                    barrierDismissible: false,
                    message: 'Incorrect \"${error}\" field, try again',
                  );
                }
                //save the _textFieldController.text to a variable
                Navigator.pop(context);
              },
              child: Text(
                "OK",
                style: kButtonText.copyWith(color: Colors.black),
              ),
            ),
          ],
        ),
      );

  String _updateData(AsyncSnapshot<DocumentSnapshot<Object?>> snap,
      TextEditingController data, String type) {
    String error = "no";
    dynamic to_update = data.text;
    if (type == "active" || type == "bridge" || type == "configured") {
      if (to_update == "true") {
        to_update = true;
      } else if (to_update == "false") {
        to_update = false;
      } else {
        return type;
      }
    }

    snap.requireData.reference.update({type: to_update});
    return error;
  }
}
