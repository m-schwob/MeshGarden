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
import 'package:iot_firestore_flutter_app/screens/sensors_screen.dart';
//import 'package:iot_firestore_flutter_app/model/Sensor.dart';
import 'package:iot_firestore_flutter_app/route/routing_constants.dart';

//import 'package:iot_firestore_flutter_app/widgets/my_sensor_card.dart';
// import 'package:flutter_hooks/flutter_hooks.dart';

class ConfigScreen extends StatefulWidget {
  const ConfigScreen({Key? key, required this.nodeId}) : super(key: key);

  final String nodeId;

  @override
  _ConfigScreenState createState() => _ConfigScreenState();
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

class _ConfigScreenState extends State<ConfigScreen> {
  late final _docStream = FirebaseFirestore.instance
      .collection('Nodes')
      .doc(widget.nodeId)
      .snapshots();

  var _active = TextEditingController();
  var _nickname = TextEditingController();
  var _bridge = TextEditingController();
  var _configured = TextEditingController();

  // final TextEditingController _password = TextEditingController();
  // final TextEditingController _passwordConfirm = TextEditingController();

  @override
  void dispose() {
    _active.dispose();
    _nickname.dispose();
    _bridge.dispose();
    _configured.dispose();
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
          // bottom : PreferredSize(
          //   preferredSize: ,
          //     child: Text("Configurations",
          //     style: kBodyText2)
          // ),
          title: Text("Configurations",
        style: kBodyText2),
          // title: UserAccountsDrawerHeader(
          //   accountName: Text("${user!.displayName}", style: kBodyText2),
          //   accountEmail: Text(""),
          // decoration: BoxDecoration(
          //   color: Colors.blue,
          // ),
          // ),
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
            // final data = snapshot.data!.docs.map((DocumentSnapshot node_document)

            // final data = data.
            return Padding(
                padding: const EdgeInsets.only(
                    left: 16, right: 16, top: 20, bottom: 30),
                child: Scaffold(
                  body: SingleChildScrollView(
                    child: Column(
                      children: [
                        Card(
                            // shape: RoundedRectangleBorder(
                            //   borderRadius: BorderRadius.circular(18),
                            // ),
                            shadowColor: Colors.white,
                            elevation: 20,
                            color: kTextFieldFill,

                            child: ListTile(
                              subtitle:
                                  Text(node_data['nickname'], style: kConfigTitle),
                              title: Text('nickname :',style: kConfigSubTitle),
                              trailing: IconButton(
                                icon: Icon(Icons.edit),
                                color: Colors.white,
                                onPressed: () {
                                  _openEditDialog(TextInputType.text, snapshot,
                                      _nickname, "nickname",node_data['nickname']);
                                },
                              ),
                            )
                        ),
                        // Card(
                        //     // shape: RoundedRectangleBorder(
                        //     //   borderRadius: BorderRadius.circular(18),
                        //     // ),
                        //     shadowColor: Colors.white,
                        //     elevation: 20,
                        //     color: kTextFieldFill,
                        //
                        //     child: ListTile(
                        //       subtitle:
                        //       Text(node_data['active'].toString(), style: kConfigTitle),
                        //       title: Text('active :',style: kConfigSubTitle),
                        //       trailing: IconButton(
                        //         icon: Icon(Icons.edit),
                        //         color: Colors.white,
                        //         onPressed: () {
                        //           _openEditDialog(TextInputType.text, snapshot,
                        //               _active, "active",node_data['active']);
                        //         },
                        //       ),
                        //     )
                        // ),
                        // Card(
                        //     // shape: RoundedRectangleBorder(
                        //     //   borderRadius: BorderRadius.circular(18),
                        //     // ),
                        //     shadowColor: Colors.white,
                        //     elevation: 20,
                        //     color: kTextFieldFill,
                        //
                        //     child: ListTile(
                        //       subtitle:
                        //       Text(node_data['bridge'].toString(), style: kConfigTitle),
                        //       title: Text('bridge :',style: kConfigSubTitle),
                        //       trailing: IconButton(
                        //         icon: Icon(Icons.edit),
                        //         color: Colors.white,
                        //         onPressed: () {
                        //           _openEditDialog(TextInputType.text, snapshot,
                        //               _bridge, "bridge",node_data['bridge']);
                        //         },
                        //       ),
                        //     )
                        // ),
                        // Card(
                        //     // shape: RoundedRectangleBorder(
                        //     //   borderRadius: BorderRadius.circular(18),
                        //     // ),
                        //     shadowColor: Colors.white,
                        //     elevation: 20,
                        //     color: kTextFieldFill,
                        //
                        //     child: ListTile(
                        //       subtitle:
                        //       Text(node_data['configured'].toString(), style: kConfigTitle),
                        //       title: Text('configured :',style: kConfigSubTitle),
                        //       trailing: IconButton(
                        //         icon: Icon(Icons.edit),
                        //         color: Colors.white,
                        //         onPressed: () {
                        //           _openEditDialog(TextInputType.text, snapshot,
                        //               _configured, "configured",node_data['configured']);
                        //         },
                        //       ),
                        //     )
                        // ),
                        Card(
                          // shape: RoundedRectangleBorder(
                          //   borderRadius: BorderRadius.circular(18),
                          // ),
                            shadowColor: Colors.white,
                            elevation: 20,
                            color: kTextFieldFill,

                            child: ListTile(
                              // subtitle:
                              // Text(node_data['configured'].toString(), style: kConfigTitle),
                              // contentPadding: EdgeInsets.symmetric(horizontal: 135.0),
                              title: Center(
                                child: Text('Sensors',style: kConfigTitle),
                              ),
                               onTap: () {
                                 Navigator.push(
                                   context,
                                   MaterialPageRoute(
                                     builder: (context) =>
                                         SensorsScreen(nodeId: widget.nodeId),
                                   ),
                                 );
                               },
                              // trailing: IconButton(
                              //   icon: Icon(Icons.edit),
                              //   color: Colors.white,
                              //   onPressed: () {
                              //     _openEditDialog(TextInputType.text, snapshot,
                              //         _configured, "configured",node_data['configured']);
                              //   },
                              // ),
                            )
                        ),
                      ],
                    ),
                  ),
                ));

            /**
                return Padding(
                padding:
                const EdgeInsets.only(left: 16, right: 16, top: 40, bottom: 30),
                child: CustomScrollView(slivers: [
                SliverFillRemaining(
                hasScrollBody: false,
                child: Column(
                children: [
                Flexible(
                child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                Padding(
                padding: const EdgeInsets.all(8.0),
                child: Text(
                //collectionName,
                "Config",
                style: kHeadline,
                ),
                ),
                Padding(
                padding: const EdgeInsets.all(8.0),
                child: Text(
                //data.docs.first.id,
                //"NodeID: ",
                widget.nodeId,
                style: kHeadline,
                ),
                ),
                SizedBox(
                height: 30,
                ),
                Center(
                child: Column(
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                MyTextField(
                hintText: 'Active',
                inputType: TextInputType.text,
                textEditingController: _active,
                ),
                MyTextField(
                hintText: 'Nickname',
                inputType: TextInputType.name,
                textEditingController: _nickname,
                ),
                // TextFormField(
                //   controller: _nickname,
                //   style: TextStyle(color: Colors.white),
                //   decoration: const InputDecoration(
                //     // icon: Icon(Icons.person),
                //     labelStyle: TextStyle(color: Colors.white),
                //     hintText: 'What do people call you?',
                //     labelText: 'Nickname *',
                //   ),
                //     // onChanged:(String? value){
                //     //   snapshot.requireData.reference.update({"nickname": _nickname.text.trim()});
                //     // },
                //     onSaved: (String? value) {
                //     snapshot.requireData.reference.update({"nickname": _nickname.text.trim()});
                //   },
                // ),
                Text(
                node_data["active"].toString(),
                style: kHeadline,
                ),
                Text(
                node_data["nickname"],
                style: kHeadline,
                ),
                MyTextButton(
                buttonName: 'Save',
                onTap: () {
                _updateData(snapshot);
                },
                bgColor: Colors.white,
                textColor: Colors.black87,
                ),
                IconButton(
                onPressed: () {
                Navigator.pop(context);
                },
                icon: Image(
                width: 24,
                color: Colors.white,
                image: AssetImage(
                'assets/images/back_arrow.png'),
                ),
                ),
                // MySensorCard(
                //   value: data.docs.first.data().humidity,
                //   unit: '%',
                //   name: 'Air Humidity',
                //   assetImage: AssetImage(
                //     'assets/images/humidity_icon.png',
                //   ),
                //   trendData: rhList!,
                //   linePoint: Colors.blueAccent,
                // ),
                SizedBox(
                height: 20,
                ),
                // MySensorCard(
                //   value: data.docs.first.data().temperature,
                //   unit: '\'C',
                //   name: 'Air Temperature',
                //   assetImage: AssetImage(
                //     'assets/images/temperature_icon.png',
                //   ),
                //   trendData: tempList!,
                //   linePoint: Colors.redAccent,
                // )
                ],
                ),
                ),
                ],
                ),
                ),
                SizedBox(
                height: 20,
                ),
                ],
                ),
                ),
                ]),
                );
             **/
            // return ListView(
            //   children: snapshot.data!.docs
            //       .map((DocumentSnapshot node_document) {
            //     Map<String, dynamic> node_data =
            //     node_document.data()! as Map<String, dynamic>;
            //     return ListTile(
            //       textColor: Colors.white,
            //       title: Text(node_data['nickname']),
            //       subtitle: Text(node_document.id),
            //onTap: _nodeConfig,

            // return Row(
            //     mainAxisAlignment: MainAxisAlignment.center,
            //     children: [
            //       Text(
            //         "Sign out of Firebase? ",
            //         style: kBodyText,
            //       ),
            //       GestureDetector(
            //         onTap: _signOut,
            //         child: Text(
            //           "Sign Out",
            //           style: kBodyText.copyWith(
            //             color: Colors.white,
            //           ),
            //         ),
            //       ),
            //     ],
            //   ),
            //   SizedBox(
            //     height: 20,
            //   ),
            //   );
            // })
            //     .toList()
            //     .cast(),
            // );
          },
        ));
  }

  Future _openEditDialog(keyboard_type, snap, control, type, current_data) => showDialog(
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
                if(error != "no"){
                  showOkAlertDialog(
                    context: context,
                    barrierDismissible : false,
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

  // _inputError(field) {
  //
  // }
}

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
