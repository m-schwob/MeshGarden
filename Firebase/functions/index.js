const _ = require('lodash');
const functions = require("firebase-functions");
const admin = require("firebase-admin");
admin.initializeApp();


exports.setNetworkConfig = functions.firestore.document('/initNetwork/network_string').onWrite(async (change, context) => {
    var snap = change.after;
    functions.logger.log(snap.get('network_string'));
    var data = JSON.parse(snap.get('network_string'));
    console.log(data);
    const batch = admin.firestore().batch();
    for (key of Object.keys(data)) {
        console.log(key);
        // console.log(data[key]);
        batch.set(admin.firestore().collection('Network').doc(key), data[key]);
    }
    batch.commit();
});

exports.onNodeWrite = functions.firestore.document('/Nodes/{node_id}').onWrite(async (change, context) => {
    // preform actions only for changes in 'sensors' field.
    const before = await change.before.get('sensors');
    const after = await change.after.get('sensors');
    functions.logger.log(`before: ${before}, after: ${after}`);
    if (!_.isEqual(before, after)) {
        const node_id = context.params.node_id;
        if (change.after.exists) {
            const global_config = await get_global_config();
            await update_changes_collection(change.after, global_config);
            await update_measurements_collection(change, node_id);
            functions.logger.log("node " + node_id + " changed");
        } else {
            functions.logger.log("node " + node_id + " deleted");
        }
    }
});

exports.onConfigWrite = functions.firestore.document('/Network/{doc}').onWrite(async (change, context) => {
    const global_config = await get_global_config();
    const nodes = admin.firestore().collection('/Nodes');
    for (doc of await nodes.listDocuments()) {
        await update_changes_collection(await doc.get(), global_config);
    }
    functions.logger.log("nodes configuration changed");
});

exports.onActivityChanged = functions.firestore.document('/MeshNetwork/active').onCreate(async (snap, context) => {
    const nodes_collection = admin.firestore().collection('Nodes');

    // get active/not active nodes from Nodes collection
    var active_before = [];
    var not_active_before = [];
    for (node of await nodes_collection.listDocuments()) {
        if ((await (await node.get()).get('active')) === true)
            active_before.push(node.id);
        else
            not_active_before.push(node.id);
    }
    // get active nodes from active document
    const active_after = Object.keys(snap.data());

    //get differences
    const to_activate = not_active_before.filter(node => active_after.includes(node) === true);
    const to_deactivate = active_before.filter(node => active_after.includes(node) === false);
    functions.logger.log(`active nodes before: [${active_before}]`);
    functions.logger.log(`not active nodes before: [${not_active_before}]`);
    functions.logger.log(`active nodes after: [${active_after}]`);
    functions.logger.log(`nodes to activate: [${to_activate}]`);
    functions.logger.log(`nodes to deactivate [${to_deactivate}]`);

    // set nodes to active
    for (var node of to_activate) {
        nodes_collection.doc(node).update({ 'active': true });
        functions.logger.log(`node ${node} has been activated`);
    }

    // set nodes to not active
    for (var node of to_deactivate) {
        nodes_collection.doc(node).update({ 'active': false });
        functions.logger.log(`node ${node} has been deactivated`);
    }
});

async function get_global_config() {
    const global_config = await admin.firestore().collection('/Network').get();
    return Object.assign({}, ...global_config.docs.map(doc => ({ [doc.id]: doc.data() })));
}

async function update_changes_collection(node_document_snap, global_config) {
    // get nodes config data
    const sensors_field = await node_document_snap.get('sensors');

    // set config to changes collections
    const doc = admin.firestore().collection('Changes').doc(node_document_snap.id);
    doc.set({ 'config': JSON.stringify({ sensors: sensors_field, ...{ network_config: global_config } }) });
}

async function update_measurements_collection(change, node_id) {
    // get sensors ids from node document if exist
    var node_sensors_ids = [];
    if (change.after.exists) {
        node_sensors = await change.after.get('sensors');
        if (node_sensors) {
            node_sensors_ids = Object.keys(node_sensors);
        }
    }
    functions.logger.log(`node ${node_id} document sensors list: [${node_sensors_ids}]`);

    // get sensors ids from node measurements document if exist
    const meas_doc = await admin.firestore().collection('Measurements').doc(node_id).get();
    var meas_sensors_ids = [];
    if (meas_doc.exists) {
        node_sensors = meas_doc.data();
        if (node_sensors) {
            meas_sensors_ids = Object.keys(node_sensors);
        }
    }
    functions.logger.log(`node ${node_id} measurements document sensors list: [${meas_sensors_ids}]`);

    //get differences
    const added = node_sensors_ids.filter(x => meas_sensors_ids.includes(x) === false)
    const deleted = meas_sensors_ids.filter(x => node_sensors_ids.includes(x) === false)
    functions.logger.log(`sensors to add: [${added}]`);
    functions.logger.log(`sensors to delete [${deleted}]`);

    //update measurements collection
    if (meas_sensors_ids.length && !node_sensors_ids.length) {
        //remove node measurements document if it exist and node does not have any sensors 
        await meas_doc.ref.delete();
        functions.logger.log(`node ${node_id} measurements document has been deleted`);
    }
    else {
        //remove deleted sensors from node measurements document
        for (var sensor_id of deleted) {
            await meas_doc.ref.update({ [sensor_id]: admin.firestore.FieldValue.delete() });
            functions.logger.log(`sensor ${sensor_id} of node ${node_id} measurements has been removed.`);
        }

        // add added sensors to node measurements document
        for (var sensor_id of added) {
            const sensor = await change.after.get(`sensors.${sensor_id}`);
            try {
                for (var [type, units] of sensor.sensor_type.map((e, i) => [e, sensor.units[i]])) {
                    await meas_doc.ref.set({ [sensor_id]: { [type]: { 'units': units } } }, { merge: true });
                    // await meas_doc.ref.update({ [`${sensor_id}.${type}`]: { 'units': units } }); //also works. keep for future reference
                }
            }
            catch (e) {
                if (e instanceof TypeError) {
                    functions.logger.log(`sensor ${sensor_id} of node ${node_id} data corruption. measurement document cannot be completely updated`);
                }
                continue;
            }
            functions.logger.log(`sensor ${sensor_id} has been added to node ${node_id} measurements `);
        }
    }
}


// get keys array of document field.
// @input: field getter function, DocumentData object
// @return: array of keys. return empty array if document or data does not exists
//usage:     meas_sensors_ids = get_keys_array(meas_doc, async ()=> {meas_doc.data()});
//usage:     meas_sensors_ids = get_keys_array(change.after, async ()=> {change.after.get('sensors')});
// async function get_keys_array(document_data, field_getter){
//     if (document_data.exists) {
//         node_sensors = field_getter();
//         if (node_sensors) {
//             return Object.keys(node_sensors);
//         }
//     }
//     return [];
// }

/*jsonToDocumenton and documentToJson are based on https://stackoverflow.com/questions/62246410/how-to-convert-a-firestore-document-to-plain-json-and-vice-versa*/
// function jsonToDocument(value) {
//     if (value == null) {
//         return { "nullValue": null };
//     } else if (!isNaN(value)) {
//         if (value.toString().indexOf(".") != -1) {
//             return { "doubleValue": value };
//         } else {
//             return { "integerValue": value };
//         }
//     } else if (value === "true" || value === "false" || typeof value == "boolean") {
//         return { "booleanValue": value };
//     } else if (Date.parse(value)) {
//         return { "timestampValue": value };
//     } else if (typeof value == "string") {
//         return { "stringValue": value };
//     } else if (value && value.constructor === Array) {
//         return { "arrayValue": { values: value.map((v) => jsonToDocument(v)) } };
//     } else if (typeof value === "object") {
//         const obj = {};
//         for (const o in value) {
//             obj[o] = jsonToDocument(value[o]);
//         }
//         return { "mapValue": { fields: obj } };
//     }
// }

// /* */
// function documentToJson(fields) {
//     const result = {};
//     for (const f in fields) {
//         const key = f;
//         const value = fields[f];
//         const isDocumentType = ["stringValue", "booleanValue", "doubleValue",
//             "integerValue", "timestampValue", "mapValue", "arrayValue", "nullValue",
//         ].find((t) => t === key);
//         if (isDocumentType) {
//             const item = ["stringValue", "booleanValue", "doubleValue",
//                 "integerValue", "timestampValue", "nullValue"]
//                 .find((t) => t === key);
//             if (item) {
//                 return value;
//             } else if ("mapValue" == key) {
//                 return documentToJson(value.fields || {});
//             } else if ("arrayValue" == key) {
//                 const list = value.values;
//                 return list ? list.map((l) => documentToJson(l)) : [];
//             }
//         } else {
//             result[key] = documentToJson(value);
//         }
//     }
//     return result;
// }