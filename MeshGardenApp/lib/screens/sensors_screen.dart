import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/model/Sensor.dart';
import 'package:iot_firestore_flutter_app/screens/add_sensor_screen.dart';
import 'package:hawk_fab_menu/hawk_fab_menu.dart';
import 'package:iot_firestore_flutter_app/const/library_sensors.dart';
import 'package:iot_firestore_flutter_app/const/image_path.dart';
class SensorsScreen extends StatefulWidget {
  const SensorsScreen({Key? key, required this.nodeId}) : super(key: key);

  final String nodeId;

  @override
  _SensorsScreenState createState() => _SensorsScreenState();
}


class _SensorsScreenState extends State<SensorsScreen> {
  late final _docStream = FirebaseFirestore.instance
      .collection('Nodes')
      .doc(widget.nodeId)
      .snapshots();

  late final _sensorDoc =
      FirebaseFirestore.instance.collection('Nodes').doc(widget.nodeId);


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

          int? max_id = 0;
          if(!sensors_list.isEmpty){
            // get max sensor id
            sensors_list.forEach((sensor) {
              if (sensor.sensor_id! >= max_id!)
                max_id = (sensor.sensor_id! + 1);
            });
          }

          if (sensors_list.isEmpty) {
            return Scaffold(
              body: HawkFabMenu(
                // openIcon: Icons.add,
                  icon: AnimatedIcons.menu_close,
                  fabColor: Colors.blue,
                  iconColor: Colors.white,
                  items: [
                    HawkFabMenuItem(
                      label: 'UV',
                      ontap: () {
                        Map<String, dynamic> sensor_json = UV_DEFAULT;
                        sensor_json["sensor_id"] = 1;
                        snapshot.requireData.reference.update({"sensors.sensor1": sensor_json});

                        ScaffoldMessenger.of(context).hideCurrentSnackBar();
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(content: Text('UV added'),
                              duration: const Duration(milliseconds: 1000)),
                        );
                      },
                      icon: const Icon(Icons.add),
                      color: Colors.blue,
                      labelColor: Colors.black,
                      labelBackgroundColor: Colors.yellow,
                    ),
                    HawkFabMenuItem(
                      label: 'DHT 22',
                      ontap: () {
                        Map<String, dynamic> sensor_json = DHT22_DEFAULT;
                        sensor_json["sensor_id"] = 1;
                        snapshot.requireData.reference.update({"sensors.sensor1": sensor_json});

                        ScaffoldMessenger.of(context).hideCurrentSnackBar();
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(content: Text('DHT 22 added'),
                          duration: const Duration(milliseconds: 1000),),
                        );
                      },
                      icon: const Icon(Icons.add),
                      color: Colors.blue,
                      labelColor: Colors.red,
                      labelBackgroundColor: Colors.blueAccent,
                    ),
                    HawkFabMenuItem(
                      label: 'Soil Moisture',
                      ontap: () {
                        Map<String, dynamic> sensor_json = SOIL_MOISTURE_DEFAULT;
                        sensor_json["sensor_id"] = 1;
                        snapshot.requireData.reference.update({"sensors.sensor1": sensor_json});

                        ScaffoldMessenger.of(context).hideCurrentSnackBar();
                        ScaffoldMessenger.of(context).showSnackBar(
                          const SnackBar(content: Text('Soil Moisture added'),
                              duration: const Duration(milliseconds: 1000)),
                        );
                      },
                      icon: const Icon(Icons.add),
                      labelColor: Colors.blue,
                      labelBackgroundColor: Colors.brown,
                    ),
                    HawkFabMenuItem(
                      label: 'New custom sensor',
                      ontap: () {

                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (context) => AddSensorScreen(
                                sensorDoc: _sensorDoc, sensorId: 1),
                          ),
                        );
                      },
                      icon: const Icon(Icons.add),
                      color: Colors.green,
                      labelColor: Colors.white,
                      labelBackgroundColor: Colors.green,
                    ),
                  ],
                body:Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    crossAxisAlignment: CrossAxisAlignment.center,
                    children: [
                          Center(
                          child:
                          Text("No sensors for this plant", style: kBodyText2.copyWith(fontSize: 26)),
                        ),
                      Center(
                        child: Image(
                          // width: 60,
                          image: AssetImage(
                            CryingBabyPlant,
                          ),
                        ),
                      ),
                    ]),
              ),
            );
          } else {
            return Scaffold(
              body: HawkFabMenu(
                // openIcon: Icons.add,
                icon: AnimatedIcons.menu_close,
                fabColor: Colors.blue,
                iconColor: Colors.white,
                items: [
                  HawkFabMenuItem(
                    label: 'UV',
                    ontap: () {
                      Map<String, dynamic> sensor_json = UV_DEFAULT;
                      sensor_json["sensor_id"] = max_id;
                      snapshot.requireData.reference.update({"sensors.sensor$max_id": sensor_json});

                      ScaffoldMessenger.of(context).hideCurrentSnackBar();
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(content: Text('UV added'),
                            duration: const Duration(milliseconds: 1000)),
                      );
                    },
                    icon: const Icon(Icons.add),
                    color: Colors.blue,
                    labelColor: Colors.black,
                    labelBackgroundColor: Colors.yellow,
                  ),
                  HawkFabMenuItem(
                    label: 'DHT 22',
                    ontap: () {
                      Map<String, dynamic> sensor_json = DHT22_DEFAULT;
                      sensor_json["sensor_id"] = max_id;
                      snapshot.requireData.reference.update({"sensors.sensor$max_id": sensor_json});

                      ScaffoldMessenger.of(context).hideCurrentSnackBar();
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(content: Text('DHT 22 added'),
                          duration: const Duration(milliseconds: 1000),),
                      );
                    },
                    icon: const Icon(Icons.add),
                    color: Colors.blue,
                    labelColor: Colors.red,
                    labelBackgroundColor: Colors.blueAccent,
                  ),
                  HawkFabMenuItem(
                    label: 'Soil Moisture',
                    ontap: () {
                      Map<String, dynamic> sensor_json = SOIL_MOISTURE_DEFAULT;
                      sensor_json["sensor_id"] = max_id;
                      snapshot.requireData.reference.update({"sensors.sensor$max_id": sensor_json});

                      ScaffoldMessenger.of(context).hideCurrentSnackBar();
                      ScaffoldMessenger.of(context).showSnackBar(
                        const SnackBar(content: Text('Soil Moisture added'),
                            duration: const Duration(milliseconds: 1000)),
                      );
                    },
                    icon: const Icon(Icons.add),
                    labelColor: Colors.blue,
                    labelBackgroundColor: Colors.brown,
                  ),
                  HawkFabMenuItem(
                    label: 'New custom sensor',
                    ontap: () {

                      Navigator.push(
                        context,
                        MaterialPageRoute(
                          builder: (context) => AddSensorScreen(
                              sensorDoc: _sensorDoc, sensorId: max_id!),
                        ),
                      );
                    },
                    icon: const Icon(Icons.add),
                    color: Colors.green,
                    labelColor: Colors.white,
                    labelBackgroundColor: Colors.green,
                  ),
                ],
                body:Padding(
                  padding: const EdgeInsets.only(
                      left: 16, right: 16, top: 20, bottom: 30),
                  child: ListView.builder(
                    itemCount: sensors_list.length,
                    itemBuilder: (BuildContext ctx, int index) {
                      List<Widget> sensorAtt = [];
                      // int types_num = sensors_list[index].sensor_type!.length;
                      List<List<String>> type_unit_list = [];
                      sensorAtt.add(
                        ListTile(
                          dense: true,
                          visualDensity: VisualDensity(vertical: -2),
                          title: Text(
                            " Sensor ID :  \t\t\t\t\t\t\t\t\t" +
                                sensors_list[index].sensor_id.toString(),
                            style: kConfigSubTitle),
                        ));
                      sensorAtt.add(Divider(color: Colors.blueGrey));
                      sensorAtt.add(
                        ListTile(
                          dense: true,
                          visualDensity: VisualDensity(vertical: -2),
                          title: Text(
                            " Hardware Info : \t\t" +
                                sensors_list[index].hardware_info.toString(),
                            style: kConfigSubTitle),
                      ));
                      sensorAtt.add(Divider(color: Colors.blueGrey));
                      sensorAtt.add(
                        ListTile(
                          dense: true,
                          visualDensity: VisualDensity(vertical: -2),
                          title: Text(
                            " Sample Interval : " +
                                sensors_list[index].sample_interval.toString(),
                            style: kConfigSubTitle),
                      ));
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
                          ListTile(
                            dense: true,
                            visualDensity: VisualDensity(vertical: -2),
                            title: Text(
                              " Type  ${i}  : " +
                                  tuple[0].toString() +
                                  "\t|\tUnits  : " +
                                  "\"" +
                                  tuple[1].toString() +
                                  "\"",
                              style: kConfigSubTitle),
                          ));
                        sensorAtt.add(Divider(color: Colors.blueGrey));
                        i++;
                      });


                      sensors_list[index].pinout!.forEach((key, value) {
                        sensorAtt.add(
                          ListTile(
                            dense: true,
                            visualDensity: VisualDensity(vertical: -2),
                            title: Text(" Sensor pin :  \"${key}\" | Controller pin :   \"${value}\"",
                              style: kConfigSubTitle),
                        ));
                        sensorAtt.add(Divider(color: Colors.blueGrey));
                      });

                      sensorAtt.removeLast();
                      // sensorAtt.add(Divider());
                      sensorAtt.add(
                        ListTile(
                          // subtitle:
                          // Text(node_data['nickname'], style: kConfigTitle),
                          trailing : Container(
                            height: 35,
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
                                _openRemoveDialog(snapshot, sensors_list[index].sensor_id.toString());
                              },
                              child: Text(
                                "Remove",
                                style: RemoveTitle,
                              ),
                            ),
                          ),
                        ),
                      );

                      return Padding(
                        // padding: const EdgeInsets.symmetric(vertical: 10.0),
                        padding: const EdgeInsets.all(12.0),
                        child: Card(
                          // shape: RoundedRectangleBorder(
                          //   borderRadius: BorderRadius.circular(18),
                          // ),
                          shadowColor: Colors.white,
                          elevation: 10,
                          color: kTextFieldFill,
                          child: Column(
                              crossAxisAlignment: CrossAxisAlignment.start,
                              children: sensorAtt),

                        ),
                      );
                    },
                  ),
                ),
              ),
            );
          }
        },
      ),
    );
  }


  Future _openRemoveDialog(snap, sensor_id_string) => showDialog(
    context: context,
    barrierDismissible: false,
    builder: (context) => AlertDialog(
      title: Text("Are you sure you want to remove?"),
      actions: [
        TextButton(
          child: Text('Confirm'),
          onPressed: () {
            snap.requireData.reference.update({"sensors.sensor${sensor_id_string}": FieldValue.delete()});
            Navigator.of(context).pop();
          },
        ),
        TextButton(
          child: Text('Cancel'),
          onPressed: () {
            Navigator.of(context).pop();
          },
        ),
      ],
    ),
  );

}
