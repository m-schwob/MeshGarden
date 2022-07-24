import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:adaptive_dialog/adaptive_dialog.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/screens/sensors_screen.dart';


class ConfigScreen extends StatefulWidget {
  const ConfigScreen({Key? key, required this.nodeId}) : super(key: key);

  final String nodeId;

  @override
  _ConfigScreenState createState() => _ConfigScreenState();
}


class _ConfigScreenState extends State<ConfigScreen> {
  late final _docStream = FirebaseFirestore.instance
      .collection('Nodes')
      .doc(widget.nodeId)
      .snapshots();


  var _nickname = TextEditingController();



  @override
  void dispose() {
    _nickname.dispose();
    super.dispose();
  }


  @override
  Widget build(BuildContext context) {
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

          title: Text("Configurations",
        style: kBodyText2),

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

                            )
                        ),
                      ],
                    ),
                  ),
                ));

          },
        ));
  }

  Future _openEditDialog(keyboard_type, snap, control, type, current_data) => showDialog(
        context: context,
        builder: (context) => AlertDialog(
          title: Text(current_data.toString()),
          content: TextField(
            keyboardType: keyboard_type,
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

}

