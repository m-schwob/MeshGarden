const functions = require("firebase-functions");
const admin = require("firebase-admin");
admin.initializeApp();

// a.initializeApp();
// const admin = a.getFirestore();


exports.onNodeWrite = functions.firestore.document('/Nodes/{node_id}').onWrite(async (change, context) => {
    const node_id = context.params.node_id;

    console.log(admin.SDK_VERSION);
    console.log(admin.firestore());
    console.log((await admin.firestore().listCollections()).toString());

    if (change.after.exists) {
        const data = change.after.data();
        const doc = admin.firestore().collection('Changes').doc(node_id);
        // const doc = admin.collection('Changes').doc(node_id);
        
        doc.set({ 'config': JSON.stringify(data) });
    } else {
        functions.logger.log("Node " + node_id + "has deleted");
    }
    
    // await update_changes_collection(change, node_id);
    // await update_measurements_collection(change, node_id);
});

async function update_measurements_collection(change, node_id) {
    // get sensors ids from node document if exist
    // var node_sensors_ids = [];
    // console.log(change.after.exists);
    // if (change.after.exists) {
    //     console.log('?');
    //     node_sensors = await change.after.get('sensors');
    //     console.log(node_sensors);
    //     if (node_sensors) {
    //         console.log('??');
    //         node_sensors_ids = Object.keys(node_sensors);
    //     }
    // }
    // console.log(node_sensors_ids);
    // functions.logger.log(`node ${node_id} document sensors list: [${node_sensors_ids}]`);

    // get sensors ids from node measurement document if exist
    // const meas_doc = await admin.firestore().collection('Nodes').doc(node_id).get();
    // var meas_doc = await doc.get().then((d)=>{return d;});;

    // // const doc = admin.firestore().collection('Nodes').doc(node_id);

    // doc.set({'1':1});
    // const meas_doc = await doc.get();
    // console.log(meas_doc.exists);

    // var meas_sensors_ids = [];
    // console.log(node_id);
    // console.log(meas_doc.exists);
    // console.log(meas_doc);
    // // if (meas_doc.exists) {
    //     // console.log('?');
    //     node_sensors = meas_doc.get('flag');
    //     console.log(node_sensors);
    //     if (node_sensors) {
    //         console.log('??');
    //         meas_sensors_ids = Object.keys(node_sensors);
    //     }
    // // }
    // console.log(meas_sensors_ids);
    // functions.logger.log(`node ${node_id} document sensors list: [${meas_sensors_ids}]`);

    // x = false;
    // if (x) {

    //     const node_doc = change.after;
    //     const node_sensors = await node_doc.get('sensors');
    //     functions.logger.log(`node ${node_id} document ${node_doc.exists ? '' : 'not '}exists`);
    //     functions.logger.log(`node ${node_id} has ${node_sensors && node_sensors ? '' : 'no '}sensors`);

    //     // get the node measurement document  if exists and get it's content if exists 
    //     const meas_doc = await (admin.firestore().collection('Measurements').doc(node_id)).get();
    //     const meas_sensors = meas_doc.data();
    //     functions.logger.log(`node ${node_id} measurement document ${meas_doc.exists ? '' : 'not '}exists`);
    //     functions.logger.log(`node ${node_id} has ${meas_sensors && node_sensors != '' ? '' : 'no '}measurements`);
    //     functions.logger.log(meas_doc.exists);
    //     functions.logger.log(node_sensors);



    //     var delete_doc = false;
    //     if (change.after.exists) {
    //         functions.logger.log(node_sensors);
    //         if (node_sensors) {
    //             // get differences
    //             const node_sensors_ids = Object.keys(node_sensors);
    //             const meas_sensors_ids = Object.keys(meas_sensors ? meas_sensors : []);
    //             const added = node_sensors_ids.filter(x => meas_sensors_ids.includes(x) === false)
    //             const deleted = meas_sensors_ids.filter(x => node_sensors_ids.includes(x) === false)
    //             functions.logger.log('added ' + added);
    //             functions.logger.log('deleted ' + deleted);

    //             // remove deleted sensors
    //             for (var sensor_id in deleted) {
    //                 doc.update({ sensor_id: firebase.firestore.FieldValue.delete() });
    //                 functions.logger.log(`sensor ${sensor_id} of node ${node_id} measurements has been removed.`);
    //             }

    //             // add added sensors
    //             for (var sensor_id in added) {
    //                 const sensor = await change.after.get(`sensors.${sensor_id}`);
    //                 for (var [type, units] of sensor.sensor_type.map((e, i) => [e, sensor.units[i]])) {
    //                     doc.update({ sensor_id: { type: { 'units': units } } });
    //                 }
    //                 functions.logger.log(`sensor ${sensor_id} has been added to node ${node_id} measurements `);
    //             }
    //         }
    //         else {
    //             delete_doc = true;
    //         }
    //     } else {
    //         delete_doc = true;
    //     }
    //     functions.logger.log("Node " + node_id + "has deleted");
    // }
}

async function update_changes_collection(change, node_id) {
    // if (change.after.exists) {
    //     const data = change.after.data();
    //     const doc = admin.firestore().collection('Changes').doc(node_id);

    //     doc.set({ 'config': JSON.stringify(data) });
    // } else {
    //     functions.logger.log("Node " + node_id + "has deleted");
    // }
}

/*jsonToDocumenton and documentToJson are based on https://stackoverflow.com/questions/62246410/how-to-convert-a-firestore-document-to-plain-json-and-vice-versa*/
function jsonToDocument(value) {
    if (value == null) {
        return { "nullValue": null };
    } else if (!isNaN(value)) {
        if (value.toString().indexOf(".") != -1) {
            return { "doubleValue": value };
        } else {
            return { "integerValue": value };
        }
    } else if (value === "true" || value === "false" || typeof value == "boolean") {
        return { "booleanValue": value };
    } else if (Date.parse(value)) {
        return { "timestampValue": value };
    } else if (typeof value == "string") {
        return { "stringValue": value };
    } else if (value && value.constructor === Array) {
        return { "arrayValue": { values: value.map((v) => jsonToDocument(v)) } };
    } else if (typeof value === "object") {
        const obj = {};
        for (const o in value) {
            obj[o] = jsonToDocument(value[o]);
        }
        return { "mapValue": { fields: obj } };
    }
}

/* */
function documentToJson(fields) {
    const result = {};
    for (const f in fields) {
        const key = f;
        const value = fields[f];
        const isDocumentType = ["stringValue", "booleanValue", "doubleValue",
            "integerValue", "timestampValue", "mapValue", "arrayValue", "nullValue",
        ].find((t) => t === key);
        if (isDocumentType) {
            const item = ["stringValue", "booleanValue", "doubleValue",
                "integerValue", "timestampValue", "nullValue"]
                .find((t) => t === key);
            if (item) {
                return value;
            } else if ("mapValue" == key) {
                return documentToJson(value.fields || {});
            } else if ("arrayValue" == key) {
                const list = value.values;
                return list ? list.map((l) => documentToJson(l)) : [];
            }
        } else {
            result[key] = documentToJson(value);
        }
    }
    return result;
}
