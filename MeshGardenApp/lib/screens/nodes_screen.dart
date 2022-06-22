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
        // title: UserAccountsDrawerHeader(
        //   accountName: Text("${user!.displayName}", style: kBodyText2),
        //   accountEmail: Text(""),
        // decoration: BoxDecoration(
        //   color: Colors.blue,
        // ),
        // ),
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

          return Padding(
              padding: const EdgeInsets.only(
                  left: 16, right: 16, top: 20, bottom: 30),
              child: ListView(
                children: snapshot.data!.docs
                    .map((DocumentSnapshot node_document) {
                      Map<String, dynamic> node_data =
                          node_document.data()! as Map<String, dynamic>;
                      // return ListTile(
                      //     shape: RoundedRectangleBorder(
                      //       borderRadius: BorderRadius.circular(18),
                      //     ),
                      //     title: Text(node_data['nickname'],
                      //         style: kBodyText.copyWith(color: Colors.white),
                      //     textScaleFactor: 2),
                      //     trailing: Icon(Icons.circle,
                      //         color: node_data['active'] ? Colors.green : Colors.red)
                      //     // Text('$value$unit',
                      //     //     style: kHeadline.copyWith(color: Colors.white)),
                      //     );
                      return Card(
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(18),
                          ),
                          shadowColor: Colors.white,
                          elevation: 10,
                          color: kTextFieldFill,
                          child: ListTile(
                            onTap: (){
                              bool has_sensors = node_data['sensors'] !=null? true:false;
                              // var sensorMesureDoc = FirebaseFirestore.instance.collection('Test2').doc("30497375570").snapshots();
                              Navigator.push(
                                context,
                                MaterialPageRoute(
                                  builder: (context) =>
                                      MeasurementsScreen(nodeId: node_document.id),
                                ),
                              );
                            },
                            leading: node_data['active']? FaIcon(FontAwesomeIcons.circleCheck, color: Colors.green)
                                : FaIcon(FontAwesomeIcons.circleXmark, color: Colors.red),
                            // leading: Icon(Icons.circle,
                            //     color: node_data['active']
                            //         ? Colors.green
                            //         : Colors.red),
                            title:
                                Text(node_data['nickname'], style: kBodyText2),
                            trailing: IconButton(
                              icon: Icon(Icons.settings),
                              color: Colors.white,
                              onPressed: () {
                                Navigator.push(
                                  context,
                                  MaterialPageRoute(
                                    builder: (context) =>
                                        ConfigScreen(nodeId: node_document.id),
                                  ),
                                );
                              },
                            ),
                          ));

                      //   textColor: Colors.white,
                      //   title: Text(node_data['nickname']),
                      //   subtitle: Text(node_document.id),
                      //   onTap: () {
                      //     Navigator.push(
                      //       context,
                      //       MaterialPageRoute(
                      //         builder: (context) =>
                      //             ConfigScreen(nodeId: node_document.id),
                      //       ),
                      //     );
                      //   },
                      // );
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

  Future<User?> _getCurrentUser() async {
    return await AuthHelper.currentUser();
  }

  _signOut() async {
    await AuthHelper.signOut();
    Navigator.pushNamedAndRemoveUntil(
        context, SplashScreenRoute, (Route<dynamic> route) => false);
  }
}

// _nodeConfig(int node_id) {
//   Navigator.push(BuildContext context, SplashScreenRoute, (Route<dynamic> route) => false);
// }
// class _NodesScreenState extends State<NodesScreen> {

// @override
// Widget build(BuildContext context) {
//   return Scaffold(
//       body: StreamBuilder<QuerySnapshot<Node>>(
//         stream: nodeRef.snapshots(),
//         builder: (context, snapshot) {
//           if (snapshot.hasError) {
//             return Center(
//               child: Text(snapshot.error.toString()),
//             );
//           }
//
//           if (!snapshot.hasData) {
//             return const Center(child: CircularProgressIndicator());
//           }
//
//           final data = snapshot.requireData;
//
//           return Padding(
//             padding:
//             const EdgeInsets.only(left: 16, right: 16, top: 40, bottom: 30),
//             child: CustomScrollView(slivers: [
//               SliverFillRemaining(
//                 hasScrollBody: false,
//                 child: Column(
//                   children: [
//                     Flexible(
//                       child: Column(
//                         crossAxisAlignment: CrossAxisAlignment.start,
//                         children: [
//                           Padding(
//                             padding: const EdgeInsets.all(8.0),
//                             child: Text(
//                               collectionName,
//                               style: kHeadline,
//                             ),
//                           ),
//                           Padding(
//                             padding: const EdgeInsets.all(8.0),
//                             child: Text(
//                               data.docs.first.id,
//                               style: kHeadline,
//                             ),
//                           ),
//                           SizedBox(
//                             height: 30,
//                           ),
//                           Center(
//                             child: Column(
//                               crossAxisAlignment: CrossAxisAlignment.center,
//                               children: [
//                                 MySensorCard(
//                                   value: data.docs.first.data().humidity,
//                                   unit: '%',
//                                   name: 'Humidity',
//                                   assetImage: AssetImage(
//                                     'assets/images/humidity_icon.png',
//                                   ),
//                                   trendData: rhList!,
//                                   linePoint: Colors.blueAccent,
//                                 ),
//                                 SizedBox(
//                                   height: 20,
//                                 ),
//                                 MySensorCard(
//                                   value: data.docs.first.data().temperature,
//                                   unit: '\'C',
//                                   name: 'Temperature',
//                                   assetImage: AssetImage(
//                                     'assets/images/temperature_icon.png',
//                                   ),
//                                   trendData: tempList!,
//                                   linePoint: Colors.redAccent,
//                                 )
//                               ],
//                             ),
//                           ),
//                         ],
//                       ),
//                     ),
//                     Row(
//                       mainAxisAlignment: MainAxisAlignment.center,
//                       children: [
//                         Text(
//                           "Sign out of Firebase? ",
//                           style: kBodyText,
//                         ),
//                         GestureDetector(
//                           onTap: _signOut,
//                           child: Text(
//                             "Sign Out",
//                             style: kBodyText.copyWith(
//                               color: Colors.white,
//                             ),
//                           ),
//                         ),
//                       ],
//                     ),
//                     SizedBox(
//                       height: 20,
//                     ),
//                   ],
//                 ),
//               ),
//             ]),
//           );
//         },
//       ));
// }

// @override
// void initState() {
//   super.initState();
//   _controller = AnimationController(vsync: this);
// }
//
// @override
// void dispose() {
//   _controller.dispose();
//   super.dispose();
// }
//
// @override
// Widget build(BuildContext context) {
//   return Container();
// }
