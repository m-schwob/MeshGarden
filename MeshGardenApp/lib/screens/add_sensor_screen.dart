import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/model/Sensor.dart';


// TODO check if needed adding "_" between words in the type
class AddSensorScreen extends StatefulWidget {
  const AddSensorScreen(
      {Key? key, required this.sensorDoc, required this.sensorId})
      : super(key: key);

  final DocumentReference<Map<String, dynamic>> sensorDoc;
  final int sensorId;

  @override
  _AddSensorScreenState createState() => _AddSensorScreenState();
}

class _AddSensorScreenState extends State<AddSensorScreen> {
  var _hardware_info = TextEditingController();
  var _sample_interval = TextEditingController();

  int _pinoutsCount = 1;
  int _typesAndUnitsCount = 1;

  Map<String, String> pinout_map ={};
  List<String> types_list = [];
  List<String> units_list = [];

  List<String> sensor_pin_list= [];
  List<String> controller_pin_list= [];


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

  @override
  Widget build(BuildContext context) {
    bool keyboardIsOpened = MediaQuery.of(context).viewInsets.bottom != 0.0;


    return Scaffold(
      floatingActionButton: keyboardIsOpened? null: FloatingActionButton.extended(
        onPressed: (){
          try {

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



            widget.sensorDoc.update({"sensors.sensor${widget.sensorId}": sensor_json});
            Navigator.pop(context);
          }
          catch(e) {print("");}

        },
        // mini: true,

        elevation: 12,
        label: Text("Submit" , style: kSubmitTitle),
        backgroundColor: Colors.white,
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


                      //Add sensor's text fields
                      return Row(
                        mainAxisAlignment: MainAxisAlignment.start,

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

                      //Add sensor's text fields
                      return Row(
                        mainAxisAlignment: MainAxisAlignment.start,

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

}
