# Vigenere-Cipher-Auto-Decoder


 * Auto Vigenere Cipher Decryption Tool
  
  This program attempts to decrypt a text file encrypted with the Vigenere cipher
  without prior knowledge of the password. It uses frequency analysis and the
  index of coincidence to deduce the password length and characters.
  
  Steps:
 * 1. Normalize the text (convert to lowercase, remove non-alphabetic characters).
 * 2. Determine the password length using the index of coincidence.
 * 3. Identify each letter of the password by analyzing letter frequencies.
 * 4. Calculate the decryption key based on the assumption that 'e' is the most frequent letter.
 * 5. Decrypt the text using the derived password.
   
  Usage: ./auto_vigenere <encrypted_file> <output_file>
  
  Note: This implementation assumes that the input text is in English and that 'e' is the most frequent letter.
 
