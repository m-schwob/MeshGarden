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
import 'package:iot_firestore_flutter_app/screens/add_sensor_screen.dart';

//import 'package:iot_firestore_flutter_app/widgets/my_sensor_card.dart';
// import 'package:flutter_hooks/flutter_hooks.dart';

//TODO : add delete button to sensor card with alert window "are you sure"?

class SensorsScreen extends StatefulWidget {
  const SensorsScreen({Key? key, required this.nodeId}) : super(key: key);

  final String nodeId;

  @override
  _SensorsScreenState createState() => _SensorsScreenState();
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

class _SensorsScreenState extends State<SensorsScreen> {
  late final _docStream = FirebaseFirestore.instance
      .collection('Nodes')
      .doc(widget.nodeId)
      .snapshots();

  late final _sensorDoc =
      FirebaseFirestore.instance.collection('Nodes').doc(widget.nodeId);

  // final TextEditingController _password = TextEditingController();
  // final TextEditingController _passwordConfirm = TextEditingController();

  // @override
  // void dispose() {
  //   _active.dispose();
  //   _nickname.dispose();
  //   // _passwordConfirm.dispose();
  //   super.dispose();
  // }

  // final data = FirebaseFirestore.instance
  //     .collection("TestNodes").doc(widget.nodeId)

  // final nodeRef = FirebaseFirestore.instance
  //     .collection("TestNodes")
  //     .withConverter<Node>(
  //   fromFirestore: (snapshots, _) => Node.fromJson(snapshots.data()!),
  //   toFirestore: (node, _) => node.toJson(),
  // );

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
        title: Text("Sensors", style: kBodyText2),
      ),
      body: StreamBuilder<DocumentSnapshot>(
        stream: _docStream,
        builder:
            (BuildContext context, AsyncSnapshot<DocumentSnapshot> snapshot) {
          if (snapshot.hasError) {
            return Center(
              child: Text(snapshot.error.toString()),
            );
          }

          if (!snapshot.hasData) {
            return const Center(child: CircularProgressIndicator());
          }

          Map<String, dynamic> node_data =
              snapshot.requireData.data() as Map<String, dynamic>;

          List<Sensor> sensors_list = [];

          if (node_data["sensors"] != null) {
            Map<String, dynamic> sensors_map =
                node_data["sensors"] as Map<String, dynamic>;

            sensors_map.forEach((key, value) {
              Sensor s = Sensor.fromJson(value);
              sensors_list.add(s);
            });
          }
          // Sensor sensor = Sensor(
          //     hardware_info: "DHT22",
          //     pinout: {"DAT": "D1"},
          //     sample_interval: 30,
          //     sensor_id: 1,
          //     sensor_type: ["Air Humidity", "Air Temperature"],
          //     units: ["%", "C"]);
          //
          // var sensor_json = sensor.toJson();

          // final data = snapshot.data!.docs.map((DocumentSnapshot node_document)

          // final data = data.
          if (sensors_list.isEmpty) {
            return Scaffold(
              floatingActionButton: FloatingActionButton(
                onPressed: () {
                  // _doc.update({"sensors.1": sensor_json});
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) =>
                          AddSensorScreen(sensorDoc: _sensorDoc, sensorId: 1),
                    ),
                  );
                  // snapshot.requireData.reference.update({"sensors.1": sensor_json});
                },
                // mini: true,
                elevation: 12,
                foregroundColor: Colors.white,
                child: Icon(
                  Icons.add,
                  size: 30,
                ),
              ),
              body: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    Center(
                      child:
                          Text("No sensors for this plant", style: kBodyText2),
                    ),
                    Center(
                      child: Image(
                        // width: 60,
                        image: AssetImage(
                          'assets/images/crying_plant_new.png',
                        ),
                      ),
                    ),
                  ]),
            );
          } else {
            return Scaffold(
              floatingActionButton: FloatingActionButton(
                onPressed: () {
                  // get max sensor id
                  int? max_id = 0;
                  sensors_list.forEach((sensor) {
                    if (sensor.sensor_id! >= max_id!)
                      max_id = (sensor.sensor_id! + 1);
                  });
                  // print("got here2 , max id = ${max_id}\n");
                  // sensor_json["sensor_id"] = max_id;
                  // print("....${sensor_json["sensor_id"]}...");
                  // _doc.update({"sensors.${max_id}": sensor_json});
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => AddSensorScreen(
                          sensorDoc: _sensorDoc, sensorId: max_id!),
                    ),
                  );
                  // snapshot.requireData.reference.update({"sensors.${max_id}": sensor_json});
                },
                // mini: true,
                elevation: 12,
                foregroundColor: Colors.white,
                child: Icon(
                  Icons.add,
                  size: 30,
                ),
              ),
              body: Padding(
                padding: const EdgeInsets.only(
                    left: 16, right: 16, top: 20, bottom: 30),
                child: ListView.builder(
                  itemCount: sensors_list.length,
                  itemBuilder: (BuildContext ctx, int index) {
                    List<Widget> sensorAtt = [];
                    // int types_num = sensors_list[index].sensor_type!.length;
                    List<List<String>> type_unit_list = [];
                    // sensorAtt.add(
                    //   ListTile(
                    //     // subtitle:
                    //     // Text(node_data['nickname'], style: kConfigTitle),
                    //     title: Text(
                    //         "Sensor ID :  \t\t\t\t\t\t\t\t\t" +
                    //             sensors_list[index].sensor_id.toString(),
                    //         style: kSensorText),
                    //   ),
                    // );
                    sensorAtt.add(
                      Text(
                          " Sensor ID :  \t\t\t\t\t\t\t\t\t" +
                              sensors_list[index].sensor_id.toString(),
                          style: kSensorText),
                    );
                    sensorAtt.add(Divider(color: Colors.blueGrey));
                    sensorAtt.add(
                      Text(
                          " Hardware Info : \t\t" +
                              sensors_list[index].hardware_info.toString(),
                          style: kSensorText),
                    );
                    sensorAtt.add(Divider(color: Colors.blueGrey));
                    sensorAtt.add(
                      Text(
                          " Sample Interval : " +
                              sensors_list[index].sample_interval.toString(),
                          style: kSensorText),
                    );
                    sensorAtt.add(Divider(color: Colors.blueGrey));
                    sensors_list[index].sensor_type!.forEach((element) {
                      // int i = 1;
                      type_unit_list.add([element, "1"]);
                    });

                    sensors_list[index].units!.asMap().forEach(
                        (index, value) => {type_unit_list[index][1] = value});
                    int i = 1;
                    type_unit_list.forEach((tuple) {
                      sensorAtt.add(
                        Text(
                            " Type  ${i}  : " +
                                tuple[0].toString() +
                                "\t|\tUnits  : " +
                                "\"" +
                                tuple[1].toString() +
                                "\"",
                            style: kSensorText),
                      );
                      sensorAtt.add(Divider(color: Colors.blueGrey));
                      i++;
                    });

                    // sensorAtt.add(
                    //   Text("Out Pins  :", style: kSensorText),
                    // );

                    sensors_list[index].pinout!.forEach((key, value) {
                      sensorAtt.add(
                        Text(" Out  \"${key}\"  pin  :   \"${value}\"",
                            style: kSensorText),
                      );
                      sensorAtt.add(Divider(color: Colors.blueGrey));
                    });

                    sensorAtt.removeLast();
                    sensorAtt.add(Divider());
                    sensorAtt.add(
                        ListTile(
                          // subtitle:
                          // Text(node_data['nickname'], style: kConfigTitle),
                          trailing : Container(
                            height: 40,
                            width: 80,
                            decoration: BoxDecoration(
                              color: Colors.redAccent,
                              borderRadius: BorderRadius.circular(18),
                            ),
                            child: TextButton(
                              style: ButtonStyle(
                                overlayColor: MaterialStateProperty.resolveWith(
                                      (states) => Colors.black12,
                                ),
                              ),
                              onPressed: () {
                                snapshot.requireData.reference.update({"sensors.${sensors_list[index].sensor_id.toString()}": FieldValue.delete()});
                              },
                              child: Text(
                                "Remove",
                                style: kConfigSubTitle,
                              ),
                            ),
                          ),
                      ),
                    );

                    return Padding(
                      // padding: const EdgeInsets.symmetric(vertical: 10.0),
                      padding: const EdgeInsets.all(12.0),
                      child: Card(
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(18),
                        ),
                        shadowColor: Colors.white,
                        elevation: 10,
                        color: kTextFieldFill,
                        child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: sensorAtt),
                        // SizedBox(width:20.0),
                        // Text(
                        //     "Sensor ID:  " +
                        //         sensors_list[index].sensor_id.toString(),
                        //     style: kBodyText2),
                        // Text(
                        //     "Hardware Info:  " +
                        //         sensors_list[index].hardware_info.toString(),
                        //     style: kBodyText2),
                        //     Text(
                        //         "Sample Interval:  " +
                        //             sensors_list[index].sample_interval.toString(),
                        //         style: kBodyText2),
                        //
                        //     sensors_list[index].sensor_type.forEach((element) {
                        //       Text(
                        //           "Sample Interval:  " +
                        //               element,
                        //           style: kBodyText2);
                        //     })
                      ),
                    );
                  },
                ),
              ),
            );
          }
        },
      ),
    );
  }
}
