import 'package:firebase_auth/firebase_auth.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/route/routing_constants.dart';
import 'package:flutter/material.dart';
import 'dart:async';
import 'package:iot_firestore_flutter_app/screens/loading_screen.dart';
import '../auth_helper.dart';

class SplashScreen extends StatelessWidget {
  const SplashScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: FutureBuilder(
      future: AuthHelper.initializeFirebase(context: context),
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.done) {
          User? user = AuthHelper.currentUser();
          if (user != null) {
            Future.delayed(Duration.zero, () async {
              // Navigator.pushNamedAndRemoveUntil(context, DashboardScreenRoute,
              Navigator.pushNamedAndRemoveUntil(context, NodesScreenRoute,
                  (Route<dynamic> route) => false);
              // Navigator.pushNamedAndRemoveUntil(context, LoadingScreenRoute,
              //         (Route<dynamic> route) => false);
            });
          } else {
            return _getScreen(context);
          }
        }
        return Center(
          child: CircularProgressIndicator(),
          // child: LoadingScreen()
        );
      },
    ));
  }

  // _getLoading(BuildContext context){
  //   return Container(
  //     padding: const EdgeInsets.all(50),
  //     child: Image(
  //       image: AssetImage(
  //         'assets/images/logo.png',
  //       ),
  //       // child:FlutterLogo(size:MediaQuery.of(context).size.height)
  //     ),);
  // }

  _getScreen(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 20, vertical: 50),
      child: Column(
        children: [
          Flexible(
            child: Column(
              children: [
                Center(
                  child: Container(
                    child: Image(
                      image: AssetImage(
                        'assets/images/logo.png',
                      ),
                      //color: Colors.white,
                    ),
                  ),
                ),
                SizedBox(
                  height: 20,
                ),
                Text(
                  "Mesh Garden",
                  style: kHeadline,
                  textAlign: TextAlign.left,
                ),
                SizedBox(
                  height: 20,
                ),
                Container(
                  width: MediaQuery.of(context).size.width * 0.8,
                  child: Text(
                    "Make your plants happy\nStart your Mesh Garden now",
                    style: kBodyText,
                    textAlign: TextAlign.center,
                  ),
                )
              ],
            ),
          ),
          Container(
              height: 60,
              width: MediaQuery.of(context).size.width * 0.8,
              decoration: BoxDecoration(
                color: Colors.grey[850],
                borderRadius: BorderRadius.circular(18),
              ),
              child: Container(
                child: TextButton(
                  style: ButtonStyle(
                    overlayColor: MaterialStateProperty.resolveWith(
                      (states) => Colors.black12,
                    ),
                  ),
                  onPressed: () {
                    Navigator.pushNamedAndRemoveUntil(context,
                        SignInScreenRoute, (Route<dynamic> route) => false);
                  },
                  child: Text(
                    'GET STARTED',
                    style: kButtonText.copyWith(color: Colors.white),
                  ),
                ),
              ))
        ],
      ),
    );
  }
}
