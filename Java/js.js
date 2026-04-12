function penjumlahan(a,b){
  hasil = a + b;
  return hasil;
}
function pengurangan(a,b){
  hasil = a - b;
  return hasil;
}
let perkalian = function(){
  hasil = 5 * 5;
  return hasil;
}
console.log(penjumlahan(1,perkalian()))

function belanja(laporan){
  barang1 = "tepung";
  barang2 = "telor";

  hasil1 = laporan(barang1); // harus menampilkan papa membawa tepung
  hasil2 = laporan(barang2); // harus menampilkan papa membawa telor

  console.log(hasil1);
  console.log(hasil2);
}

// buat variabel berisi fungsi untuk menampilkan "papa pulang membawa(barang1)"

let papa = function(barang){
  return `Papa membawa pulang ${barang  }`;
}

belanja(papa);

// let delay = 1000;
// setTimeout(()=>{
//   for (let i = 0; i < 5; i++)
//     {console.log("naon nanaonan ke-", delay);
//       delay += 1000;
//     }
// }, delay)

// let angka = 1;
// let namainterval = setInterval(function(){
//   console.log(angka);
//   angka++;
  
//   if (angka == 10){
//     clearInterval(namainterval)
//   }
// },1000);

setTimeout(()=>{
console.log("DUAR!");
}, 10000)

let angka = 1;
let namainterval = setInterval(function(){
  console.log(angka);
  angka++;
  
  if (angka > 20){
    clearInterval(namainterval)
  }
},1000);