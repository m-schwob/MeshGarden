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

import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/model/Node.dart';
import 'package:iot_firestore_flutter_app/auth_helper.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/model/Sensor.dart';
import 'package:iot_firestore_flutter_app/route/routing_constants.dart';
import 'package:iot_firestore_flutter_app/screens/config_screen.dart';

import 'package:font_awesome_flutter/font_awesome_flutter.dart';

//import 'package:iot_firestore_flutter_app/widgets/my_sensor_card.dart';
import 'package:iot_firestore_flutter_app/widgets/node_card.dart';
import 'package:iot_firestore_flutter_app/screens/measurements_screen.dart';
import '../const/custom_colors.dart';

class MeasurementsScreen extends StatefulWidget {
  const MeasurementsScreen({
    Key? key,
    required this.nodeId,
  }) : super(key: key);

  final String nodeId;

  @override
  _MeasurementsScreenState createState() => _MeasurementsScreenState();
}

class _MeasurementsScreenState extends State<MeasurementsScreen> {
  final Stream<QuerySnapshot> _measureStream =
      FirebaseFirestore.instance.collection('Measurements').snapshots();

  // final Stream<QuerySnapshot> _documentStream =
  // FirebaseFirestore.instance.collection('Test2').snapshots();

  String user_email = AuthHelper.currentUser()?.email ?? "";

  // User? user;
  // FirebaseUser user = FirebaseAuth.getInstance().getCurrentUser();
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: kMainBG,
        elevation: 10.0,
        title: Text(
            user_email.substring(0, user_email.indexOf("@")) + "'s plants",
            style: kBodyText2),
      ),
      body: StreamBuilder<QuerySnapshot>(
        stream: _measureStream,
        builder: (BuildContext context, AsyncSnapshot<QuerySnapshot> snapshot) {
          if (snapshot.hasError) {
            return Center(
              child: Text(snapshot.error.toString()),
            );
          }

          if (!snapshot.hasData) {
            return const Center(child: CircularProgressIndicator());
          }

          Map<String, dynamic> sensor_data={};

          snapshot.data!.docs.forEach((node_document) {
            print(widget.nodeId + "\n");
            print(node_document.id + "------\n");
            if (node_document.id.trim() == widget.nodeId.trim()) {
              sensor_data = node_document.data()! as Map<String, dynamic>;
            }
          });

          // snapshot.data!.docs.map((DocumentSnapshot node_document){
          //   print(widget.nodeId + "\n");
          //   print(node_document.id + "------\n");
          //   if(node_document.id.trim() == widget.nodeId.trim()){
          //     sensor_data = node_document.data()! as Map<String, dynamic>;
          //   }
          // });

          print(sensor_data);


          return Center(
            child: Text("Hi", style: kSensorInputText),
          );
        },
      ),
      bottomNavigationBar: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Text(
            "Sign out of Firebase? ",
            style: kBodyText,
          ),
          GestureDetector(
            onTap: _signOut,
            child: Text(
              "Sign Out",
              style: kBodyText.copyWith(
                color: Colors.white,
              ),
            ),
          ),
        ],
      ),
    );
  }

  Future<User?> _getCurrentUser() async {
    return await AuthHelper.currentUser();
  }

  _signOut() async {
    await AuthHelper.signOut();
    Navigator.pushNamedAndRemoveUntil(
        context, SplashScreenRoute, (Route<dynamic> route) => false);
  }
}

/**
    class MeasurementsScreen extends StatefulWidget {
    const MeasurementsScreen({Key? key, required this.nodeId,required this.stream}) : super(key: key);

    final String nodeId;
    final stream;

    @override
    _MeasurementsScreenState createState() => _MeasurementsScreenState();
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

    class _MeasurementsScreenState extends State<MeasurementsScreen> {
    late final _docMeasureStream =
    FirebaseFirestore.instance.collection('Test2').doc("30497375561").snapshots();


    // late final _sensorDoc =
    // FirebaseFirestore.instance.collection('Nodes').doc(widget.nodeId);

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
    title: Text("Measurements", style: kBodyText2),
    ),
    body: Text(_docMeasureStream., style: kBodyText2),
    /*body: StreamBuilder<DocumentSnapshot>(
    stream: _docMeasureStream,
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

    // if(snapshot.data!.exists){
    //   return const Center(child: Text("Not Exists", style: kConfigSubTitle));
    // }

    Map<String, dynamic> node_data =
    snapshot.requireData.data() as Map<String, dynamic>;

    print(node_data["new"]);

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
    trailing: Container(
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
    snapshot.requireData.reference.update({
    "sensors.${sensors_list[index].sensor_id.toString()}":
    FieldValue.delete()
    });
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
    ),*/
    );
    }
    }
 */
