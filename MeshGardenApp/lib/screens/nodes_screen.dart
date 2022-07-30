import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:flutter/rendering.dart';

import 'package:iot_firestore_flutter_app/auth_helper.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';

import 'package:iot_firestore_flutter_app/route/routing_constants.dart';
import 'package:iot_firestore_flutter_app/screens/config_screen.dart';

import 'package:font_awesome_flutter/font_awesome_flutter.dart';

import 'package:iot_firestore_flutter_app/screens/measurements_screen.dart';
import '../const/custom_colors.dart';
import '../const/image_path.dart';

class NodesScreen extends StatefulWidget {
  const NodesScreen({Key? key}) : super(key: key);

  @override
  _NodesScreenState createState() => _NodesScreenState();
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

class _NodesScreenState extends State<NodesScreen> {
  final Stream<QuerySnapshot> _nodesStream =
      FirebaseFirestore.instance.collection('Nodes').snapshots();

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
        stream: _nodesStream,
        builder: (BuildContext context, AsyncSnapshot<QuerySnapshot> snapshot) {
          if (snapshot.hasError) {
            return Center(
              child: Text(snapshot.error.toString()),
            );
          }

          if (!snapshot.hasData) {
            return const Center(child: CircularProgressIndicator());
          }

          if(snapshot.data!.docs.isEmpty){
            return Container(
              child: Column(
                  mainAxisAlignment: MainAxisAlignment.center,
                  crossAxisAlignment: CrossAxisAlignment.center,
                  children: [
                    Center(
                      child:
                      Text("No Nodes for display", style: kBodyText2.copyWith(fontSize: 26)),
                    ),
                    Center(
                      child: Image(
                        // width: 60,
                        image: AssetImage(
                          BoredBabyPlant,
                        ),
                      ),
                    ),
                  ]),
            );
          }
          else return Padding(
              padding: const EdgeInsets.only(
                  left: 16, right: 16, top: 20, bottom: 30),
              child: ListView(
                children: snapshot.data!.docs
                    .map((DocumentSnapshot node_document) {
                      Map<String, dynamic> node_data =
                          node_document.data()! as Map<String, dynamic>;
                      bool is_active;
                      bool is_bridge;
                      if(node_data['active'] == null || node_data['bridge'] == null){
                        node_data['nickname'] = "ERROR, Don't use!";
                        is_active = false;
                        is_bridge = false;
                      }
                      else{
                        is_active = node_data['active'];
                        is_bridge = node_data['bridge'];
                      }
                      num battery_level = node_data['battery'] == null
                          ? 100
                          : node_data['battery'] as num;
                      battery_level = battery_level.toInt();

                      return Container(
                        padding: EdgeInsets.all(3),
                        // decoration: new BoxDecoration(
                        //   boxShadow: [
                        //     new BoxShadow(
                        //       color: Colors.black,
                        //       blurRadius: 5.0,
                        //     ),
                        //   ],
                        // ),
                        child: Card(
                            shape: RoundedRectangleBorder(
                              borderRadius: BorderRadius.circular(18),
                            ),
                            shadowColor: Colors.white,
                            elevation: 8,
                            color: kTextFieldFill,
                            child: Column(
                              children: [
                                ListTile(
                                    dense: true,
                                    visualDensity: VisualDensity(vertical: -3),
                                    trailing: is_bridge
                                        ? Wrap(
                                            crossAxisAlignment:
                                                WrapCrossAlignment.center,
                                            // alignment: WrapAlignment.spaceEvenly,
                                            spacing: 2, // space between two icons
                                            children: <Widget>[
                                              FaIcon(FontAwesomeIcons.wifi,
                                                  color: Colors
                                                      .blueAccent),
                                              SizedBox(width: 6,),
                                              ActiveIcon(isActive: is_active),// icon-1
                                              IconButton(
                                                iconSize: 30,
                                                icon: BatteryCard(
                                                    level: battery_level),
                                                color: Colors.white,
                                                onPressed: () {},
                                              ),
                                              Text(
                                                battery_level.toString() + "%",
                                                style: BatteryLevel,
                                              ), // icon-2
                                            ],
                                          )
                                        : Wrap(
                                            crossAxisAlignment:
                                                WrapCrossAlignment.center,
                                            // alignment: WrapAlignment.spaceEvenly,
                                            spacing: 2, // space between two icons
                                            children: <Widget>[
                                              ActiveIcon(isActive: is_active),
                                              IconButton(
                                                iconSize: 30,
                                                icon: BatteryCard(
                                                    level: battery_level),
                                                color: Colors.white,
                                                onPressed: () {},
                                              ),
                                              Text(
                                                battery_level.toString() + "%",
                                                style: BatteryLevel,
                                              ), // icon-2
                                            ],
                                          )),
                                ListTile(
                                  onTap: () {
                                    bool has_measurements = true;
                                    if (node_data['sensors'] == null) {
                                      has_measurements = false;
                                    } else {
                                      Map<String, dynamic> check_sensors_map =
                                          node_data["sensors"]
                                              as Map<String, dynamic>;
                                      if (check_sensors_map.isEmpty) {
                                        has_measurements = false;
                                      }
                                    }
                                    Navigator.push(
                                      context,
                                      MaterialPageRoute(
                                        builder: (context) => MeasurementsScreen(
                                            nodeId: node_document.id,
                                            hasMeasurements: has_measurements),
                                      ),
                                    );
                                  },
                                  title: node_data['nickname'] != null? Text(node_data['nickname'], style: kBodyText2) :
                                  Text("ERROR, Don't use!",style: kBodyText2),
                                ),
                                ListTile(
                                  dense: true,
                                  visualDensity: VisualDensity(vertical: -2),
                                  trailing: IconButton(
                                    icon: Icon(Icons.settings),
                                    color: Colors.white,
                                    onPressed: () {
                                      Navigator.push(
                                        context,
                                        MaterialPageRoute(
                                          builder: (context) => ConfigScreen(
                                              nodeId: node_document.id),
                                        ),
                                      );
                                    },
                                  ),
                                ),
                              ],
                            )),
                      );
                    })
                    .toList()
                    .cast(),
              ));
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

  _signOut() async {
    await AuthHelper.signOut();
    Navigator.pushNamedAndRemoveUntil(
        context, SplashScreenRoute, (Route<dynamic> route) => false);
  }
}

class ActiveIcon extends StatelessWidget {
  const ActiveIcon({
    Key? key,
    required this.isActive,
  }) : super(key: key);

  final bool isActive;

  @override
  Widget build(BuildContext context) {
    if (isActive) {
      return FaIcon(FontAwesomeIcons.circleCheck, color: Colors.green);
    }
    return FaIcon(FontAwesomeIcons.circleXmark, color: Colors.red);
  }
}

class BatteryCard extends StatelessWidget {
  const BatteryCard({
    Key? key,
    required this.level,
  }) : super(key: key);

  final num level;

  @override
  Widget build(BuildContext context) {
    level.toInt();
    if (50 <= level && level <= 74) {
      return FaIcon(FontAwesomeIcons.batteryThreeQuarters,
          size: 32, color: Colors.lightGreen);
    } else if (25 <= level && level <= 49) {
      return FaIcon(FontAwesomeIcons.batteryHalf,
          size: 32, color: Color(0xffFF7F7F));
    } else if (1 <= level && level <= 24) {
      return FaIcon(FontAwesomeIcons.batteryQuarter,
          size: 32, color: Colors.red);
    } else if (0 == level) {
      return FaIcon(FontAwesomeIcons.batteryEmpty,
          size: 32, color: Colors.black);
    }
    return FaIcon(FontAwesomeIcons.batteryFull, size: 32, color: Colors.green);
  }
}
